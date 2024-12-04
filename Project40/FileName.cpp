#include <iostream>
#include <string>

using namespace std;

const int MAX_TAGS = 1000;  

struct Stack {
    string tags[MAX_TAGS];
    int top = -1;

    void push(const string& tag) {
        if (top < MAX_TAGS - 1) {
            tags[++top] = tag;
        }
    }

    string pop() {
        if (top >= 0) {
            return tags[top--];
        }
        return "";
    }

    bool isEmpty() {
        return top == -1;
    }

    string peek() {
        if (top >= 0) {
            return tags[top];
        }
        return "";
    }
};

string getTag(const string& line, size_t& pos) {
    if (line[pos] == '<') {
        size_t closePos = line.find('>', pos);
        if (closePos != string::npos) {
            string tag = line.substr(pos + 1, closePos - pos - 1);
            pos = closePos;
            return tag;
        }
    }
    return "";
}

bool isValidHTML(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    Stack tagStack;
    string line;

    while (getline(file, line)) {
        for (size_t i = 0; i < line.length(); ++i) {
            if (line[i] == '<') {
                string tag = getTag(line, i);

                if (!tag.empty() && tag[0] == '/') {  
                    if (tagStack.isEmpty() || tagStack.peek() != tag.substr(1)) {
                        file.close();
                        return false;
                    }
                    tagStack.pop();
                }
                else if (!tag.empty() && tag[tag.length() - 1] != '/') {  
                    tagStack.push(tag);
                }
            }
        }
    }

    file.close();
    return tagStack.isEmpty();
}

int main() {
    string filePath;
    cout << "Enter the path to the HTML file: ";
    cin >> filePath;

    if (isValidHTML(filePath)) {
        cout << "The HTML file is valid." << endl;
    }
    else {
        cout << "The HTML file is invalid." << endl;
    }

    return 0;
}
