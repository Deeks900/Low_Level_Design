#include <iostream>
#include <vector> 
#include <string> 
#include <memory>
#include <fstream>
using namespace std;

/* ================================
   Base Class : DocumentElement
================================ */
class DocumentElement{
    public:
        //pure virtual function
        virtual string render() = 0;
};

/* ================================
   Text Element
================================ */
class TextElement : public DocumentElement { 
    string text;

    public: 
    //constructor
    TextElement(string text) {
        this->text = text;
    }

    string render() override{
        return text;
    }
};

/* ================================
   Image Element
================================ */
class ImageElement : public DocumentElement { 
    string imagePath;

    public:
    ImageElement(string imagePath) {
        this->imagePath = imagePath;
    }

    string render() override {
        return "[Image: " + imagePath + "]";
    }
};

/* ================================
   Formatting Elements
================================ */
class NewLineElement : public DocumentElement {
public:
    string render() override {
        return "\n";
    }
};

class TabSpaceElement : public DocumentElement {
public:
    string render() override {
        return "\t";
    }
};

/* ================================
   Document (COMPOSITION)
================================ */
//All the crud operations over the document will be performed here
class Document {
    vector<DocumentElement*> elements;

    public:
        void addElement(DocumentElement* element) {
            elements.push_back(element);
        }

        vector<DocumentElement*> getElements() {
            return elements;
        }
};

/* ================================
   Document Renderer
================================ */
class DocumentRenderer {
public:
    string render(Document doc) {

        string result;

        for (auto element : doc.getElements()) {
            result += element->render();
        }

        return result;
    }
};

/* ================================
   Persistence Interface
================================ */
class Persistence {
public:
    //Pure virtual function
    virtual void save(string data) = 0;
};

/* ================================
   File Storage (Inheritance)
================================ */
// FileStorage implementation of Persistence
class FileStorage : public Persistence {
public:
    void save(string data) override {
        ofstream outFile("document.txt");
        if (outFile) {
            outFile << data;
            outFile.close();
            cout << "Document saved to document.txt" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

/* ================================
   DB Storage (Dummy)
================================ */
class DBStorage : public Persistence {
public:
    void save(string data) override {
        // Save to DB
    }
};

/* ================================
   Document Editor
================================ */
class DocumentEditor {
    Document* document;
    Persistence* storage;

public:
    DocumentEditor(Document* document, Persistence* storage) {
        this->document = document;
        this->storage = storage;
    }

    void addText(string text) {
        document->addElement(new TextElement(text));
    }

    void addImage(string path) {
        document->addElement(new ImageElement(path));
    }

    void addNewLine() {
        document->addElement(new NewLineElement());
    }

    void addTab() {
        document->addElement(new TabSpaceElement());
    }

    void save(string renderedData) {
        storage->save(renderedData);
    }
};

/* ================================
   Client Code
================================ */
int main() {

    Document* doc = new Document();
    Persistence* storage = new FileStorage();

    DocumentEditor editor(doc, storage);
    DocumentRenderer renderer;

    editor.addText("Hello LLD");
    editor.addNewLine();
    editor.addTab();
    editor.addText("Renderer separated!");
    editor.addNewLine();
    editor.addImage("cat.png");

    string output = renderer.render(*doc);

    cout << output << endl;

    editor.save(output);

    delete doc;
    delete storage;

    return 0;
}






