#include "pugixml-master/src/pugixml.hpp"
#include "pugixml-master/src/pugixml.cpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <filesystem>  
#include <ctime>

using namespace std::chrono;
using namespace std;

struct XMLGeneratorConfig {
    long long size;
    std::vector<std::string> validKeys;
    std::vector<std::string> validValues;
    int maxDepth;
    int maxKeySize;
    int maxValueSize;
    int maxAttrSize;
    bool allowRepeatedKeys;
    bool allowSelfClosing;
    bool includeAttributes;
    int maxAttributesPerElement;
    bool useEscapedChars;
};

class XMLGenerator {
public:
    XMLGenerator(const XMLGeneratorConfig& cfg) : config(cfg) {
        rng.seed(static_cast<unsigned int>(std::time(nullptr)));
    }

    void generate(pugi::xml_node& parent, int currentDepth = 0) {
        if (currentSize >= config.size || currentDepth > config.maxDepth) return;

        int childrenCount = std::min(3LL, config.size - currentSize);
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
    long long currentSize = 0;
    std::mt19937 rng;

    std::string randomKey() {
        return truncate(randomFrom(config.validKeys), config.maxKeySize);
    }

    std::string inflate(const std::string& base, size_t minSize) {
        std::string result = base;
        while (result.size() < minSize) {
            result += base;
        }
        return result.substr(0, minSize);
    }
    

    std::string randomValue() {
        std::string val = truncate(randomFrom(config.validValues), config.maxValueSize);
        val = inflate(val, config.maxValueSize); // Inflate here
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
    cfg.size = 80000000;
    cfg.validKeys = {"item", "data", "entry", "book", "author", "title", "price", "description", "category", 
        "tag", "university", "department", "course", "student", "professor", "class", "subject", "grade", "year", 
        "semester", "assignment", "project", "lab", "experiment", "research", "publication", "conference", "journal",
        "article", "paper", "thesis", "dissertation", "report", "presentation", "lecture", "tutorial", "workshop",
        "seminar", "meeting", "discussion", "forum", "chat", "message", "email", "notification", "alert", "reminder",
        "task", "to-do", "calendar", "event", "schedule", "timeline", "milestone", "goal", "objective", "target",
        "achievement", "success", "failure", "error", "bug", "issue", "problem", "solution", "fix", "update", "upgrade",
        "patch", "version", "release", "build", "deployment", "installation", "configuration", "setup", "environment",
        "system", "network", "server", "client", "database", "table", "row", "column", "field", "record", "entry",
        "key", "value", "pair", "mapping", "relation", "link", "connection", "association", "reference", "pointer",
        "handle", "identifier", "name", "label", "tag", "marker", "flag", "sign", "symbol", "icon", "image", "picture",
        "photo", "graphic", "illustration", "drawing", "sketch", "design", "layout", "template", "format", "style"};
    cfg.validValues = {"value1", "value2", "sample<text>", "value&special", "example\"text", "another'example", "text with spaces", 
        "text with <tags>", "text with & special chars", "text with \"quotes\"", "text with 'single quotes'", "text with < and >",
        "text with & and \"", "text with ' and &", "text with < and '", "text with > and \"", "text with < and &", "text with ' and >",
        "text with \" and <", "text with ' and <", "text with \" and >", "text with ' and >", "text with < and \"", "text with > and '",
        "Jack", "Jill", "John", "Jane", "Doe", "Smith", "Brown", "Davis", "Miller", "Wilson", "Moore", "Taylor", "Anderson", "Thomas",
        "Jackson", "White", "Harris", "Martin", "Thompson", "Garcia", "Martinez", "Robinson", "Clark", "Rodriguez", "Lewis", "Lee",
        "Walker", "Hall", "Allen", "Young", "Hernandez", "King", "Wright", "Lopez", "Hill", "Scott", "Green", "Adams", "Baker",
        "Gonzalez", "Nelson", "Carter", "Mitchell", "Perez", "Roberts", "Turner", "Phillips", "Campbell", "Parker", "Evans", "Edwards",
        "Collins", "Stewart", "Sanchez", "Morris", "Rogers", "Reed", "Cook", "Morgan", "Bell", "Murphy", "Bailey", "Rivera", "Cooper",
        "Richardson", "Cox", "Howard", "Ward", "Torres", "Peterson", "Gray", "Ramirez", "James", "Watson", "Brooks", "Kelly", "Sanders",
        "Price", "Bennett", "Wood", "Barnes", "Ross", "Henderson", "Coleman", "Jenkins", "Perry", "Powell", "Long", "Patterson", "Hughes",
        "Flores", "Washington", "Butler", "Simmons", "Foster", "Gonzales", "Bryant", "Alexander", "Russell", "Griffin", "Diaz", "Hayes"};
    cfg.maxDepth = 3;
    cfg.maxKeySize = 8;
    cfg.maxValueSize = 2000;
    cfg.maxAttrSize = 512;
    cfg.allowRepeatedKeys = true;
    cfg.allowSelfClosing = false;
    cfg.includeAttributes = true;
    cfg.maxAttributesPerElement = 2;
    cfg.useEscapedChars = true;

    pugi::xml_document doc;
    auto root = doc.append_child("root");

    XMLGenerator gen(cfg);
    gen.generate(root);

    doc.save_file("output.xml");

    std::cout << "XML generated and saved to output.xml" << std::endl;
    // After doc.save_file("output.xml");
    std::cout << "File size: " 
    << std::filesystem::file_size("output.xml") / (1024 * 1024) 
    << " MB" << std::endl;
    return 0;
}
