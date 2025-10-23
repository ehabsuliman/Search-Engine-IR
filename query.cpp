#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <regex>
#include <filesystem>
#include <cctype> // Added for isspace

using namespace std;
namespace fs = std::filesystem;

// Hardcoded stop words
const unordered_set<string> STOP_WORDS = {
    "a", "an", "and", "are", "as", "at", "be", "by", "for", "from", "has", "he",
    "in", "is", "it", "its", "of", "on", "that", "the", "to", "was", "were", "will",
    "with", "i", "you", "we", "they", "this", "but", "or", "not", "all", "any",
    "can", "do", "had", "have", "if", "no", "one", "out", "so", "up", "what", "when"
};

struct Posting {
    int docId;
    vector<int> positions;
};

using Index = unordered_map<string, vector<Posting>>;

void loadIndex(Index& index, const string& indexFile = "/Users/mac2/University/Information Retrieval/Implementation/PII/Search-Engine-IR/pos_inverted_index.json") {
    ifstream in(indexFile);
    if (!in) {
        cerr << "Error: File " << indexFile << " not found" << endl;
        return;
    }
    string line;
    int line_number = 0;
    while (getline(in, line)) {
        ++line_number;
        // Remove leading and trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        if (line.empty()) {
            cout << "Warning: Empty line at line " << line_number << " in " << indexFile << endl;
            continue;
        }
        try {
            istringstream iss(line);
            char c;
            string term;
            string valueStr;
            if (iss >> ws >> c && c == '{') {
                string key;
                iss >> ws;
                getline(iss, key, ':');
                if (key.empty() || key[0] != '"') {
                    cout << "Error: Invalid JSON format at line " << line_number << ": " << line << endl;
                    continue;
                }
                term = key.substr(1, key.length() - 2); // Remove quotes
                getline(iss, valueStr, '}');
                valueStr = valueStr.substr(1); // Remove opening bracket
            } else {
                cout << "Error: Invalid JSON format at line " << line_number << ": " << line << endl;
                continue;
            }
            vector<string> valueParts;
            istringstream valueIss(valueStr);
            string part;
            while (getline(valueIss, part, ',')) {
                valueParts.push_back(part);
            }
            if (valueParts.size() < 2 || valueParts[1].empty()) {
                cout << "Error: Invalid JSON structure at line " << line_number << ": " << line << endl;
                continue;
            }
            string docPosStr = valueParts[1]; // Skip count, take {docId: [positions]}
            istringstream docPosIss(docPosStr);
            string docEntry;
            vector<Posting> term_postings;
            while (getline(docPosIss, docEntry, '}')) {
                if (docEntry.empty() || docEntry.find('{') == string::npos) continue;
                docEntry = docEntry.substr(docEntry.find('{') + 1); // Remove opening brace
                size_t colonPos = docEntry.find(':');
                if (colonPos == string::npos) continue;
                // Extract docId (remove quotes)
                size_t docIdStart = docEntry.find('"');
                if (docIdStart == string::npos || docIdStart >= colonPos) continue;
                size_t docIdEnd = docEntry.find('"', docIdStart + 1);
                if (docIdEnd == string::npos || docIdEnd > colonPos) continue;
                string docIdStr = docEntry.substr(docIdStart + 1, docIdEnd - docIdStart - 1);
                // Extract positions (get content inside [])
                size_t posStart = docEntry.find('[', colonPos);
                if (posStart == string::npos) continue;
                size_t posEnd = docEntry.find(']', posStart);
                if (posEnd == string::npos) continue;
                string posStr = docEntry.substr(posStart + 1, posEnd - posStart - 1);
                try {
                    int docId = stoi(docIdStr);
                    vector<int> positions;
                    istringstream posIss(posStr);
                    string pos;
                    while (getline(posIss, pos, ',')) {
                        if (!pos.empty()) {
                            pos.erase(remove_if(pos.begin(), pos.end(), [](unsigned char c) { return isspace(c); }), pos.end()); // Remove spaces
                            positions.push_back(stoi(pos));
                        }
                    }
                    if (!positions.empty()) {
                        term_postings.push_back({docId, positions});
                    }
                } catch (const exception& e) {
                    cout << "Error: Invalid docId or positions at line " << line_number << ": " << line << endl;
                    continue;
                }
            }
            if (!term_postings.empty()) {
                index[term] = term_postings;
            }
        } catch (const exception& e) {
            cout << "Error: Malformed JSON at line " << line_number << ": " << line << " (" << e.what() << ")" << endl;
            continue;
        }
    }
    if (index.empty()) {
        cout << "Warning: No valid entries loaded from " << indexFile << endl;
    }
}

void loadMapping(unordered_map<int, string>& docMapping, const string& mappingFile = "/Users/mac2/University/Information Retrieval/Implementation/PII/Search-Engine-IR/docId_filePath_mapping.csv") {
    ifstream in(mappingFile);
    if (!in) {
        cerr << "Error: File " << mappingFile << " not found" << endl;
        return;
    }
    string line;
    getline(in, line); // Skip header
    int line_number = 2;
    while (getline(in, line)) {
        istringstream iss(line);
        string docIdStr, path;
        if (getline(iss, docIdStr, ',') && getline(iss, path)) {
            try {
                docMapping[stoi(docIdStr)] = path;
            } catch (const exception& e) {
                cout << "Warning: Invalid docId at line " << line_number << ": " << line << endl;
                continue;
            }
        } else {
            cout << "Warning: Invalid CSV row at line " << line_number << ": " << line << endl;
        }
        ++line_number;
    }
}

set<int> processPhraseQuery(const string& query, const Index& index) {
    string q = query;
    transform(q.begin(), q.end(), q.begin(), ::tolower);
    regex wordRegex(R"(\b\w+\b)");
    sregex_iterator iter(q.begin(), q.end(), wordRegex);
    sregex_iterator end;
    vector<string> queryTerms;
    for (; iter != end; ++iter) {
        string token = iter->str();
        if (STOP_WORDS.count(token) || token.length() < 3) continue;
        queryTerms.push_back(token);
    }
    if (queryTerms.empty()) return {};
    set<int> candidateDocs;
    unordered_map<string, unordered_map<int, vector<int>>> termPostings;
    for (const auto& term : queryTerms) {
        auto it = index.find(term);
        if (it == index.end()) return {};
        set<int> docs;
        for (const auto& p : it->second) {
            docs.insert(p.docId);
            termPostings[term][p.docId] = p.positions;
        }
        if (candidateDocs.empty()) {
            candidateDocs = docs;
        } else {
            set<int> intersection;
            set_intersection(candidateDocs.begin(), candidateDocs.end(), docs.begin(), docs.end(),
                             inserter(intersection, intersection.begin()));
            candidateDocs = intersection;
        }
    }
    set<int> matchingDocs;
    for (int docId : candidateDocs) {
        vector<vector<int>> posLists;
        for (const auto& term : queryTerms) {
            posLists.push_back(termPostings[term][docId]);
        }
        for (int startPos : posLists[0]) {
            bool match = true;
            for (size_t i = 1; i < queryTerms.size(); ++i) {
                int expectedPos = startPos + static_cast<int>(i);
                if (find(posLists[i].begin(), posLists[i].end(), expectedPos) == posLists[i].end()) {
                    match = false;
                    break;
                }
            }
            if (match) {
                matchingDocs.insert(docId);
                break;
            }
        }
    }
    return matchingDocs;
}

int main() {
    cout << "Loading index and mapping..." << endl;
    Index index;
    loadIndex(index);
    if (index.empty()) {
        cout << "Failed to load index. Exiting." << endl;
        return 1;
    }
    unordered_map<int, string> docMapping;
    loadMapping(docMapping);
    if (docMapping.empty()) {
        cout << "Failed to load mapping. Exiting." << endl;
        return 1;
    }
    while (true) {
        cout << "Enter a phrase query (or 'exit' to quit): ";
        string query;
        getline(cin, query);
        if (query == "exit") break;
        auto matchingDocs = processPhraseQuery(query, index);
        cout << "Number of matching documents: " << matchingDocs.size() << endl;
        if (!matchingDocs.empty()) {
            cout << "Matching document paths:" << endl;
            for (int docId : matchingDocs) {
                auto it = docMapping.find(docId);
                if (it != docMapping.end()) {
                    cout << it->second << endl;
                } else {
                    cout << "Unknown docId " << docId << endl;
                }
            }
        } else {
            cout << "No matching documents found." << endl;
        }
    }
    return 0;
}