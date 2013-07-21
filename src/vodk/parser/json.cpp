
#include "json.hpp"
#include <string.h>
#include <stdio.h>
#include <assert.h>

namespace vodk {
namespace json {

static const int EXPECT_UNKNOW   = 0;
static const int EXPECT_VALUE    = 1 << 0;
static const int EXPECT_NAME     = 1 << 1;
static const int EXPECT_COLON    = 1 << 2;
static const int EXPECT_COMMA    = 1 << 3;
static const int EXPECT_END      = 1 << 4;
static const int STOP            = 1 << 5;
static const int CAN_RESET       = 1 << 6;


static void printAdvancement(const char* str, int len, int pos) {
    int i = 0;
    printf("\033[1m");
    while (i < len) {
        if (i == pos) {
            printf("\033[1;31m");
        } else if (i == pos + 1) {
            printf("\033[0;28m");
        }
        printf("%c", str[i]);
        ++i;
    }
    printf("\033[0m\n");
}

static void printNodes(const Node* n) {
    if (n == nullptr) {
        return;
    }
    printNodes(n->getParent());
    if (n->is_in_array()) {
        printf(".[%i]", n->get_index());
    } else if (n->is_in_Object()) {
        printf(".%s", n->get_name().c_str());
    } else {
        //printf("<%i>", n->get_type());
    }
}

static bool parseIdentifier(const char* src, const char* val, int maxLength, int* nReadChars) {
    *nReadChars = 0;
    for (unsigned int i = 0; i < maxLength; ++i) {
        if (*val == '\0') {
            return true;
        }
        if (*src != *val) {
            return false;
        }
        ++src;
        ++val;
        ++(*nReadChars);
    }
    return *val == '\0';
}

bool parseNumber(const char* src, int maxChars, double* output, int* nReadChars) {
    enum State {
        SIGN, INTEGRAL, DECIMAL, STOP
    };
    State state = SIGN;
    double sign = 1.0;
    double value = 0.0;
    double decimals = 0.1;
    bool stop = false;
    bool status = false;
    * nReadChars = 0;
    while (*nReadChars < maxChars) {
        if (*src == '\0') {
            break;
        }
        switch (state) {

            case SIGN: {
                if (*src == ' ') {
                    break;
                }
                if (*src == '-') {
                    sign = -1.0;
                    break;
                }
                // do not break
            }

            case INTEGRAL: {
                if (*src >= '0' && *src <= '9') {
                    value *= 10.0;
                    value += static_cast<double>(*src - '0');
                    state = INTEGRAL;
                    status = true;
                    break;
                }
                if (*src == '.') {
                    state = DECIMAL;
                    break;
                }
                stop = true;
                break;
            }

            case DECIMAL: {
                if (*src >= '0' && *src <= '9') {
                    value += (double)(*src - '0') * decimals;
                    decimals *= 0.1;
                    status = true;
                    break;
                }
                stop = true;
                break;
            }

            case STOP: {
                stop = true;
            }

        } // switch
        if (stop) {
            break;
        }
        ++src;
        ++*nReadChars;
    }

    if (status) {
        *output = value * sign;
        return true;        
    }
    *nReadChars = 0;
    *output = 0.0;
    return false;
}



Parser::Parser()
: _currentNode(nullptr)
, _currentIndex(-1)
, _state(EXPECT_VALUE|CAN_RESET)
, _nextID(0)
{}

bool Parser::parse(const std::string& src) {
    return parse(src.c_str(), src.size());
}

bool Parser::parse(const char* src, int nChars) {
    const char* it = src;
    const char* prev_it = it;
    int i = 0;
    _state &= ~CAN_RESET;
    while (it < src + nChars) {
        if (_state == STOP) {
            _state |= CAN_RESET;
            return false;
        }
        while (*it == ' ' || *it == '\t' || *it == '\n') {
            ++it;
            if (it > src + nChars) {
                _state |= CAN_RESET;
                return true;
            }
        }

        //printAdvancement(src, nChars, it - src);

        if (*it == '{') {
            assert(_state & EXPECT_VALUE);
            _state = EXPECT_NAME | EXPECT_END;
            _currentNode = new json::Node(++_nextID,
                                          _currentNode,
                                          TYPE_OBJECT,
                                          _currentName,
                                          _currentIndex);
            on_object_start(_currentNode);
            ++it;
            continue;
        }
        if (*it == '}') {
            on_object_end(_currentNode);
            if (!_currentNode || _currentNode->get_type() != TYPE_OBJECT) {
                on_error(ERROR_UNEXPECTED_OBJECT_END);
            }
            pop_node();
            ++it;
            continue;
        }
        if (*it == '[') {
            assert(_state == EXPECT_VALUE);
            _state = EXPECT_VALUE;
            _currentNode = new json::Node(++_nextID,
                                          _currentNode,
                                          TYPE_ARRAY,
                                          _currentName,
                                          _currentIndex);
            _currentIndex = 0;
            on_array_start(_currentNode);
            ++it;
            continue;
        }
        if (*it == ']') {
            on_array_end(_currentNode);
            if (!_currentNode || _currentNode->get_type() != TYPE_ARRAY) {
                on_error(ERROR_UNEXPECTED_ARRAY_END);
            }
            pop_node();
            ++it;
            continue;
        }
        if (*it == ',') {
            if (!(_state & EXPECT_COMMA)) {
                on_error(ERROR_UNEXPECTED_COMMA);
                abort();
            }
            if (is_in_Object()) {
                _state = EXPECT_NAME;
                _currentName = "";
            } else if (is_in_array()) {
                _state = EXPECT_VALUE;
                ++_currentIndex;
            }
            ++it;
            continue;
        }
        if (*it == ':') {
            if (!(_state & EXPECT_COLON)) {
                on_error(ERROR_UNEXPECTED_COLON);
                abort();
            }
            _state = EXPECT_VALUE;
            ++it;
            continue;
        }
        if (*it == '"') {
            const char* it2 = it + 1;
            while (*it2 != '"') {
                if (it2 >= src + nChars) {
                    on_error(ERROR_UNTERMINATED_STRING);
                    abort();
                }
                if (*it2 == '\\') {
                    ++it2;
                }
                ++it2;
            }
            if (_state & EXPECT_NAME) {
                _state = EXPECT_COLON;
                _currentName = std::string(it + 1, it2 - it - 1);
            } else if (_state & EXPECT_VALUE) {
                _state = EXPECT_COMMA|EXPECT_END;
                Node n(0,_currentNode, TYPE_STRING, _currentName, _currentIndex);
                n._str = std::string(it + 1, it2 - it - 1);
                on_value(&n);
            } else {
                on_error(ERROR_UNEXPECTED_STRING);
                abort();
            }

            it = it2 + 1;
            continue;
        }
        int nReadChars = 0;
        if (parseIdentifier(it,"true", nChars - (src - it), &nReadChars)) {
            assert(_state & EXPECT_VALUE);
            _state = EXPECT_COMMA|EXPECT_END;
            Node n(++_nextID, _currentNode, TYPE_BOOLEAN, _currentName, _currentIndex);
            n._boolean = true;
            on_value(&n);
            while (nReadChars > 0) {
                ++it;
                --nReadChars;
            }
            continue;
        }
        if (parseIdentifier(it,"false", nChars - (src - it), &nReadChars)) {
            assert(_state & EXPECT_VALUE);
            _state = EXPECT_COMMA|EXPECT_END;
            Node n(++_nextID, _currentNode, TYPE_BOOLEAN, _currentName, _currentIndex);
            n._boolean = false;
            on_value(&n);
            while (nReadChars > 0) {
                ++it;
                --nReadChars;
            }
            continue;
        }
        if (parseIdentifier(it,"null", nChars - (src - it), &nReadChars)) {
            assert(_state & EXPECT_VALUE);
            _state = EXPECT_COMMA|EXPECT_END;
            Node n(++_nextID, _currentNode, TYPE_NULL, _currentName, _currentIndex);
            on_value(&n);
            while (nReadChars > 0) {
                ++it;
                --nReadChars;
            }
            continue;
        }
        double val = 0.0;
        if (parseNumber(it, nChars - (src - it), &val, &nReadChars)) {
            if (!(_state & EXPECT_VALUE)) {
                on_error(ERROR_UNEXPECTED_VALUE);
                abort();
            }
            _state = EXPECT_COMMA|EXPECT_END;
            Node n(++_nextID, _currentNode, TYPE_NUMBER, _currentName, _currentIndex);
            n._number = val;
            on_value(&n);
            while (nReadChars > 0) {
                ++it;
                --nReadChars;
            }
            continue;
        }
        on_error(ERROR_SYNTAX);
        _state |= CAN_RESET;
        return false;
    }
    _state |= CAN_RESET;
    return true;
}

void Parser::abort() {
    _state = STOP;
}

void Parser::reset() {
    assert(_state & CAN_RESET);
    on_reset();
    _state = EXPECT_VALUE|CAN_RESET;
    _currentIndex = -1;
    _nextID = 0;
    while (_currentNode) {
        Node* n = _currentNode;
        delete _currentNode;
        _currentNode = n->_parent;
    }
}

bool Parser::is_finished() const {
    return _currentNode == nullptr;
}

bool Parser::is_in_array() const
{
    return _currentNode ? _currentNode->get_type() == TYPE_ARRAY
                        : false;
}

bool Parser::is_in_Object() const
{
    return _currentNode ? _currentNode->get_type() == TYPE_OBJECT
                        : false;
}

void Parser::pop_node()
{
    Node* n = _currentNode;
    _currentIndex = n->get_index();
    _currentName = "";
    _currentNode = n->_parent;
    delete n;
    if (_currentNode != nullptr) {
        _state = EXPECT_COMMA|EXPECT_END;
    }
}

void Parser::on_error(json::Error err) {
    printf("Error <%i>\n", err);
    abort();
}

class Minifier : public json::Parser {
public:
    virtual void on_object_start(const json::Node* n) {
        maybeComa(n);
        maybeName(n);
        printf("{");
        firstElt = true;
    }
    virtual void on_object_end(const  json::Node* n) {
        printf("}");
    }
    virtual void on_array_start(const json::Node* n) {
        maybeComa(n);
        maybeName(n);
        printf("[");
    }
    virtual void on_array_end(const json::Node* n) {
        printf("]");
    }
    virtual void on_value(const json::Node* n) {
        maybeComa(n);
        maybeName(n);
        switch (n->get_type()) {
            case TYPE_BOOLEAN:
                printf(n->get_boolean()?"true":"false");
                break;
            case TYPE_STRING:
                printf("\"%s\"", n->get_string().c_str());
                break;
            case TYPE_NULL:
                printf("null");
                break;
            case TYPE_NUMBER:
                printf("%f", n->get_number());
                break;
            default:
                assert("unexpected"=="type");
        }
    }
    virtual void on_reset() {
        firstElt = false;
    }
    void maybeComa(const Node* n) {
        if (n->is_in_Object() && !firstElt) {
            printf(",");
        } else if (n->is_in_array() && n->get_index() > 0) {
            printf(",");
        }
        firstElt = false;
    }
    void maybeName(const Node* n) {
        if (n->is_in_Object()) {
            printf("\"%s\":", n->get_name().c_str());
        }
    }

    Minifier() : firstElt(false) {}
    bool firstElt;
};

class Formater : public json::Parser {
public:
    virtual void on_object_start(const json::Node* n) {
        maybeComa(n);
        indent();
        maybeName(n);
        printf("{");
        firstElt = true;
        ++indentation;
    }
    virtual void on_object_end(const  json::Node* n) {
        --indentation;
        indent();
        printf("}");
    }
    virtual void on_array_start(const json::Node* n) {
        maybeComa(n);
        indent();
        maybeName(n);
        printf("[");
        ++indentation;
    }
    virtual void on_array_end(const json::Node* n) {
        --indentation;
        indent();
        printf("]");
    }
    virtual void on_value(const json::Node* n) {
        maybeComa(n);
        indent();
        maybeName(n);
        switch (n->get_type()) {
            case TYPE_BOOLEAN:
                printf(n->get_boolean()?"true":"false");
                break;
            case TYPE_STRING:
                printf("\"%s\"", n->get_string().c_str());
                break;
            case TYPE_NULL:
                printf("null");
                break;
            case TYPE_NUMBER:
                printf("%f", n->get_number());
                break;
            default:
                assert("unexpected"=="type");
        }
    }
    virtual void on_reset() {
        firstElt = false;
    }
    void maybeComa(const Node* n) {
        if (n->is_in_Object() && !firstElt) {
            printf(",");
        } else if (n->is_in_array() && n->get_index() > 0) {
            printf(",");
        }
        firstElt = false;
    }
    void maybeName(const Node* n) {
        if (n->is_in_Object()) {
            printf("\"%s\": ", n->get_name().c_str());
        }
    }
    void indent() {
        printf("\n");
        for (int i = 0; i < indentation*nSpaces; ++i) {
            printf(" ");
        }
    }

    Formater(int numSpaces) : firstElt(false), indentation(0), nSpaces(numSpaces) {}
    bool firstElt;
    int indentation;
    int nSpaces;
};


} // json
} // vodk

#ifdef JSON_TEST_MAIN

int main(int argc, char** argv) {

    //json::Parser p;
    //p.parse(argv[1], strlen(argv[1]));
    double val = 0;
    int read = 0;
    assert(json::parseNumber("1,2", 4, &val, &read));
    printf("read:%i", read);
    assert(read == 1);
    printf("parse 1: %f\n", val);
    assert(json::parseNumber("10", 2, &val, &read));
    assert(read == 2);
    printf("parse 10: %f\n", val);
    assert(json::parseNumber("10.0", 4, &val, &read));
    assert(read == 4);
    printf("parse 10.0: %f\n", val);
    assert(json::parseNumber("3.1415", 6, &val, &read));
    printf("parse 3.1415: %f\n", val);
    assert(json::parseNumber("-1.5", 4, &val, &read));
    printf("parse -1.5: %f\n", val);
    assert(!json::parseNumber("foo", 4, &val, &read));


    json::Formater m(2);
    m.reset();
    assert(m.parse(argv[1], strlen(argv[1])));
    assert(m.is_finished());
    m.reset();
    m.reset();
    printf("\n");

    return 0;
}

#endif
