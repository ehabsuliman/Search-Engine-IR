# 📘 Positional Inverted Index Search Engine (C++)

This project implements a **Positional Inverted Index** using C++.  
It indexes a collection of text documents and supports **phrase-based queries**, allowing you to search for exact sequences of words like `"information retrieval system"`.

---

## 🚀 Features

- 🔍 Builds a **positional inverted index** from a folder of text files  
- 🧠 Stores the index in JSON format (`pos_inverted_index.json`)  
- 💬 Supports **phrase queries** (exact word sequence search)  
- 🧹 Automatically removes common **stop words**  
- 🔗 Maps each document ID to its **file path** using `docId_filePath_mapping.csv`  
- 🧾 Uses a **command-line interface** for interactive searching  

---

## 📂 Project Structure

📁 Search-Engine-IR/
├── main.cpp # Main source file
├── pos_inverted_index.json # Generated positional inverted index
├── docId_filePath_mapping.csv # Document ID → file path mapping
└── 📄 documents/ # Folder containing your text files


---

## 🧠 What Is a Positional Inverted Index?

A **positional inverted index** stores, for each word (term), the list of documents and the exact **positions** where the term occurs.

Example entry:
```json
{"happy": [4, {1: [156, 985, 658]}, {66: [30]}, {90: [775, 89]}, {27: [15, 120, 128]}]}

Explanation:

"happy" → indexed term

4 → appears in 4 documents

{1: [156, 985, 658]} → in document 1, appears at positions 156, 985, and 658

{66: [30]} → in document 66, at position 30

…and so on

⚙️ How It Works

1. Index Loading → Reads the JSON file pos_inverted_index.json and reconstructs the index in memory.

2. Mapping → Loads document IDs and paths from docId_filePath_mapping.csv.

3. Phrase Query → When you enter a phrase:

  - Tokenizes and lowercases the query

  - Removes stop words

  - Finds documents containing all query terms

  - Verifies if words appear in the same order and adjacent positions

4. Output → Displays the count and file paths of matching documents.

$ ./positional_index_search
Loading index and mapping...
Enter a phrase query (or 'exit' to quit): happy today
Number of matching documents: 2
Matching document paths:
/Users/mac2/University/Information Retrieval/Docs/doc_1.txt
/Users/mac2/University/Information Retrieval/Docs/doc_9.txt

Enter a phrase query (or 'exit' to quit): exit


| File                         | Description                                         |
| ---------------------------- | --------------------------------------------------- |
| `main.cpp`                   | Core logic for index loading, phrase query, and CLI |
| `pos_inverted_index.json`    | JSON-based positional inverted index                |
| `docId_filePath_mapping.csv` | Document ID to file path mapping                    |
| `documents/`                 | Folder with your text files to be indexed           |

🛠️ Dependencies

Built entirely with the C++ Standard Library:

<filesystem>

<unordered_map>, <set>, <regex>, <vector>, <string>

✅ Requires C++17 or later
✅ No external libraries required

🧾 Compilation & Execution
1️⃣ Compile
g++ -std=c++17 main.cpp -o positional_index_search

2️⃣ Run
./positional_index_search


Make sure these files exist before running:

pos_inverted_index.json

docId_filePath_mapping.csv
