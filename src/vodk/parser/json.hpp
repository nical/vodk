
#ifndef PARSER_JSON_HPP
#define PARSER_JSON_HPP

#include <string>
#include <stdint.h>
#include <assert.h>

#define nullptr 0

namespace vodk {
namespace json {

enum Type {
    TYPE_UNKNOWN, TYPE_OBJECT, TYPE_ARRAY, TYPE_BOOLEAN, TYPE_NULL, TYPE_STRING, TYPE_NUMBER
};

enum Error {
    ERROR_UNEXPECTED_OBJECT_END,
    ERROR_UNEXPECTED_ARRAY_END,
    ERROR_UNEXPECTED_COLON,
    ERROR_UNEXPECTED_COMMA,
    ERROR_UNEXPECTED_STRING,
    ERROR_UNEXPECTED_VALUE,
    ERROR_UNTERMINATED_STRING,
    ERROR_SYNTAX
};

class Node;

class Parser {
public:
    Parser();

    bool parse(const std::string& src);
    bool parse(const char* src, int nChars);
    void reset();
    bool isFinished() const;
protected:
    virtual void onObjectStart(const json::Node*) {}
    virtual void onObjectEnd(const  json::Node*) {}
    virtual void onArrayStart(const json::Node*) {}
    virtual void onArrayEnd(const json::Node*) {}
    virtual void onValue(const json::Node*) {}
    virtual void onError(json::Error err);
    virtual void onReset() {}

    void abort();

private:
    bool isInArray() const;
    bool isInObject() const;
    void popNode();

    json::Node* _currentNode;
    std::string _currentName;
    int _currentIndex;
    int _state;
    uint64_t _nextID;
};


class Node {
public:
    json::Type getType() const {return _type; }
    const std::string& getName() const { return _name; }
    const int getIndex() const { return _index; }
    uint64_t getID() const { return _id; }

    const Node* getParent() const { return _parent; }
    bool isInArray() const { return _parent ? _parent->_type == TYPE_ARRAY : false; }
    bool isInObject() const { return _parent ? _parent->_type == TYPE_OBJECT : false; }
    int getDepth() const { return _parent ? _parent->getDepth() + 1 : 0; }

    bool isBoolean() const { return _type == TYPE_BOOLEAN; }
    bool isNumber() const { return _type == TYPE_NUMBER; }
    bool isString() const { return _type == TYPE_STRING; }
    bool isNull() const { return _type == TYPE_NULL; }

    const bool getBoolean() const {
        assert(isBoolean());
        return _boolean;
    }
    const double getNumber() const {
        assert(isNumber());
        return _number;
    }
    const std::string& getString() const {
        assert(isString());
        return _str;
    }

protected:
    Node(uint64_t id, Node* parentNode, Type nodeType, const std::string& nodeName, int idx)
    : _id(id)
    , _parent(parentNode)
    , _type(nodeType)
    , _name(nodeName)
    , _index(idx)
    {}

    uint64_t    _id;
    json::Node* _parent;
    json::Type  _type;
    std::string _name;
    int         _index;
    bool        _boolean;
    std::string _str;
    double      _number;
    friend class Parser;
};

bool readNumber(const char* src, int maxChars, int* nCharsRead);


} // json
} // vodk

#endif
