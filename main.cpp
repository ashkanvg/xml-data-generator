#include "pugixml-master/src/pugixml.hpp"
#include "pugixml-master/src/pugixml.cpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <ctime>

using namespace std::chrono;
using namespace std;

struct XMLGeneratorConfig {
    int size;                                   // Maximum size of the XML document             
    std::vector<std::string> validKeys;         // List of valid keys for XML elements
    std::vector<std::string> validValues;       // List of valid values for XML elements
    std::vector<std::string> validAttributes;   // List of valid attributes for XML elements
    int maxDepth;                               // Maximum depth of the XML tree
    int maxKeySize;                             // Maximum size of keys
    int maxValueSize;                           // Maximum size of values
    int maxAttrSize;                            // Maximum size of attributes
    int maxAttributesPerElement;                // Maximum number of attributes per element
    bool allowRepeatedKeys;                     // Allow repeated keys
    bool allowSelfClosing;                      // Allow self-closing tags      
    bool includeAttributes;                     // Include attributes in elements
    bool useEscapedChars;                       // Use escaped characters in values
};

class XMLGenerator {
public:
    XMLGenerator(const XMLGeneratorConfig& cfg) : config(cfg) {
        rng.seed(static_cast<unsigned int>(std::time(nullptr)));
    }

    void generate(pugi::xml_node& parent, int currentDepth = 0) {
        if (currentSize >= config.size || currentDepth > config.maxDepth) return;

        int childrenCount = std::min(3, config.size - currentSize);
        for (int i = 0; i < childrenCount; ++i) {
            std::string key = randomKey();
            pugi::xml_node node;

            if (config.allowSelfClosing && randBool()) {
                node = parent.append_child(key.c_str());
                currentSize++;
                continue;
            }

            node = parent.append_child(key.c_str());
            if (config.includeAttributes) {
                int attrCount = rand() % (config.maxAttributesPerElement + 1);
                for (int j = 0; j < attrCount; ++j) {
                    node.append_attribute(randomKey().c_str()) = randomAttribute().c_str();
                }
            }

            std::string value = randomValue();
            if (!value.empty()) node.text().set(value.c_str());
            currentSize++;

            generate(node, currentDepth + 1);
        }
    }

private:
    XMLGeneratorConfig config;
    int currentSize = 0;
    std::mt19937 rng;

    std::string randomKey() {
        return truncate(randomFrom(config.validKeys), config.maxKeySize);
    }

    std::string randomValue() {
        std::string val = truncate(randomFrom(config.validValues), config.maxValueSize);
        if (config.useEscapedChars) {
            replaceAll(val, "&", "&amp;");
            replaceAll(val, "<", "&lt;");
            replaceAll(val, ">", "&gt;");
            replaceAll(val, "\"", "&quot;");
            replaceAll(val, "'", "&apos;");
        }
        return val;
    }

    std::string randomAttribute() {
        return truncate(randomValue(), config.maxAttrSize);
    }

    std::string truncate(const std::string& str, size_t maxSize) {
        return (str.size() > maxSize) ? str.substr(0, maxSize) : str;
    }

    std::string randomFrom(const std::vector<std::string>& list) {
        if (list.empty()) return "";
        std::uniform_int_distribution<> dist(0, list.size() - 1);
        return list[dist(rng)];
    }

    bool randBool() {
        std::uniform_int_distribution<> dist(0, 1);
        return dist(rng);
    }

    void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
};

int main() {
    XMLGeneratorConfig cfg;
    cfg.size = 100;
    cfg.validKeys = {"item", "data", "entry", "book", "author", "title", "price", "description", "category", "tag", "university", "department", "course", "student", "professor", "class", "subject", "grade", "year", "semester"};
    cfg.validValues = {"value1", "value2", "sample<text>", "value&special", "example\"text", "another'example", "text with spaces", "text with <tags>", "text with & special chars"};
    cfg.maxDepth = 5;
    cfg.maxKeySize = 10;
    cfg.maxValueSize = 15;
    cfg.maxAttrSize = 10;
    cfg.allowRepeatedKeys = true;
    cfg.allowSelfClosing = true;
    cfg.includeAttributes = true;
    cfg.maxAttributesPerElement = 2;
    cfg.useEscapedChars = true;

    pugi::xml_document doc;
    auto root = doc.append_child("root");

    XMLGenerator gen(cfg);
    gen.generate(root);

    doc.save_file("output.xml");

    std::cout << "XML generated and saved to output.xml" << std::endl;
    return 0;
}