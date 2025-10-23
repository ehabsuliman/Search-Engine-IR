Positional Inverted Index Search Engine
Overview
This project implements a Positional Inverted Index Search Engine designed for information retrieval tasks. The system allows users to input a query, search for it, and retrieve documents containing the query terms. It uses a positional inverted index to store term positions within documents and a CSV file to map document IDs to their file paths.
Features

Query Processing: Enter a query to search for terms across a collection of documents.
Positional Inverted Index: Stores terms and their positions in documents for efficient retrieval.
Document Mapping: Uses a CSV file to map document IDs to file paths.
Supported Languages: Includes C++ and Python implementations for query processing.

File Structure
Positional Inverted Index/
├── .vscode/
│   ├── launch.json        # VS Code debugging configuration
│   └── tasks.json         # VS Code tasks configuration
├── documents/
│   ├── doc1.txt           # Sample document 1
│   ├── doc2.txt           # Sample document 2
│   ├── doc3.txt           # Sample document 3
│   ├── doc4.txt           # Sample document 4
├── HW1.docx               # Project documentation or assignment details
├── PII.ipynb              # Jupyter Notebook for index creation or analysis
├── docId_filePath_mapping.csv  # Maps document IDs to file paths
├── pos_inverted_index.json     # Positional inverted index data
├── query                  # Compiled query executable
├── query.cpp              # C++ source code for query processing
├── query.py               # Python source code for query processing
└── ~$HW1.docx             # Temporary Word file (can be ignored)

Setup Instructions

Clone the Repository:
git clone git@github.com:username/repository.git
cd Positional Inverted Index

Replace username/repository with your GitHub username and repository name.

Install Dependencies:

Python: Ensure Python 3.x is installed. Install required packages (if any):pip install -r requirements.txt

(Note: Add a requirements.txt file if your Python script uses external libraries like pandas or numpy.)
C++: Ensure a C++ compiler (e.g., g++) is installed. Compile the query.cpp file:g++ query.cpp -o query




Run the Search Engine:

Using Python:python query.py


Using C++:./query



Follow the prompts to enter a query and retrieve matching documents.

Data Files:

Ensure docId_filePath_mapping.csv and pos_inverted_index.json are in the project root.
Place documents in the documents/ directory for indexing.



Usage

Run the query executable or query.py script.
Input a search query when prompted.
The system returns document IDs and paths (from docId_filePath_mapping.csv) containing the query terms, using the positional inverted index (pos_inverted_index.json).

Notes

The ~$HW1.docx file is a temporary file and can be safely ignored or deleted.
The Jupyter Notebook (PII.ipynb) may contain code for building the index or analyzing results.
Ensure all document files in documents/ are plain text for proper indexing.

Contributing

Fork the repository.
Create a new branch (git checkout -b feature-branch).
Make changes and commit (git commit -m "Add feature").
Push to the branch (git push origin feature-branch).
Open a pull request on GitHub.

License
This project is licensed under the MIT License.
