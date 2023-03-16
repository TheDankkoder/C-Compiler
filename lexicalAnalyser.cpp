
#include <bits/stdc++.h>

using namespace std;

// Token types
enum TokenType
{
    INT,
    FLOAT,
    CHAR,
    STRING,
    IDENTIFIER,
    MAIN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGN,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    AND,
    OR,
    NOT,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMICOLON,
    COMMA,
    SLASH,
    END_OF_FILE,
    ERROR,
    INCREMENT
};

vector<string> mapping = {"INT",
                          "FLOAT",
                          "CHAR",
                          "STRING",
                          "IDENTIFIER",
                          " MAIN",
                          "PLUS",
                          " MINUS",
                          "MULTIPLY",
                          "DIVIDE",
                          "MODULO",
                          "ASSIGN",
                          "EQUAL",
                          "NOT_EQUAL",
                          "LESS_THAN",
                          "LESS_THAN_OR_EQUAL",
                          "GREATER_THAN",
                          "GREATER_THAN_OR_EQUAL",
                          "AND",
                          "OR",
                          "NOT",
                          "LEFT_PAREN",
                          "RIGHT_PAREN",
                          "LEFT_BRACE",
                          "RIGHT_BRACE",
                          "SEMICOLON",
                          "COMMA",
                          "SLASH",
                          "END_OF_FILE",
                          "ERROR",
                          "INCREMENT"
                          };

// Token structure
class Token
{
public:
    TokenType type;
    string lexeme;
    int line;

    Token(TokenType type, string lexeme, int line)
    {
        this->type = type;
        this->lexeme = lexeme;
        this->line = line;
    }
};

// Lexer class
class Lexer
{
public:
    Lexer(string program_name);

    vector<Token> getTokens();
    void printTokens();
    void outputToFile();

private:
    void addToken(TokenType type, string lexeme);
    void scanToken();
    

    std::string program_name;
    std::string input;
    bool isDigit(char c);
    bool isAlpha(char c);

    bool isAlphaNumeric(char c);
    char advance();
    char peek();
    char peekNext();
    bool match(char expected);

    void number();
    void identifier();
    void string();

    void comment();

    int currentLine = 1;
    int currentColumn = 0;
    int start = 0;
    int current = 0;
    vector<Token> tokens;
};


// Constructor
Lexer::Lexer(std::string program_name)
{
    this->program_name = program_name;
    std::ifstream is(program_name);
    
    std::stringstream buffer;
    buffer << is.rdbuf();
    this->input  = buffer.str();
    cout<< this->input <<endl;
    


}

// Get the list of tokens
vector<Token> Lexer::getTokens()
{

    while (current < input.length())
    {
        start = current;
        scanToken();
    }

    addToken(END_OF_FILE, "");
    return tokens;
}

// Add a token to the token list
void Lexer::addToken(TokenType type, std::string lexeme)
{
    Token token(type, lexeme, currentLine);

    tokens.push_back(token);
}

// Scan the next token
void Lexer::scanToken()
{
    char c = advance();

    switch (c)
    {
    // Single-character tokens
    case '+':
        if(match('+'))
        {
            addToken(INCREMENT,"++");

        }
        else
        {
            addToken(PLUS, "+");
        }
        break;
    case '-':
        addToken(MINUS, "-");
        break;
    case '*':
        addToken(MULTIPLY, "*");
        break;
    case '/':
        if (match('/'))
        {
            // Single-line comment, ignore rest of line
            while (peek() != '\n' && peek() != '\0')
            {
                advance();
            }
        }
        else if (match('*'))
        {
            // Multi-line comment
            while (peek() != '\0')
            {
                if (peek() == '*' && peekNext() == '/')
                {
                    advance(); // Consume the ''
                    advance(); // Consume the '/'
                    break;
                }
                else
                {
                    if (peek() == '\n')
                    {
                        currentLine++;
                        currentColumn = 1;
                    }
                    advance();
                }
            }
        }
        else
        {
            addToken(DIVIDE, "/");
        }
        break;
    case '%':
        addToken(MODULO, "%");
        break;
    case '=':
        if (match('='))
        {
            addToken(EQUAL, "==");
        }
        else
        {
            addToken(ASSIGN, "=");
        }
        break;
    case '!':
        if (match('='))
        {
            addToken(NOT_EQUAL, "!=");
        }
        else
        {
            addToken(NOT, "!");
        }
        break;
    case '<':
        if (match('='))
        {
            addToken(LESS_THAN_OR_EQUAL, "<=");
        }
        else
        {
            addToken(LESS_THAN, "<");
        }
        break;
    case '>':
        if (match('='))
        {
            addToken(GREATER_THAN_OR_EQUAL, ">=");
        }
        else
        {
            addToken(GREATER_THAN, ">");
        }
        break;
    case '&':
        if (match('&'))
        {
            addToken(AND, "&&");
        }
        else
        {
            addToken(ERROR, "Unexpected character");
        }
        break;

    case '|':
        if (match('|'))
        {
            addToken(OR, "||");
        }
        else
        {
            addToken(ERROR, "Unexpected character");
        }
        break;
    case '(':
        addToken(LEFT_PAREN, "(");
        break;
    case ')':
        addToken(RIGHT_PAREN, ")");
        break;
    case '{':
        addToken(LEFT_BRACE, "{");
        break;
    case '}':
        addToken(RIGHT_BRACE, "}");
        break;
    case ';':
        addToken(SEMICOLON, ";");
        break;
    case ',':
        addToken(COMMA, ",");
        break;

    // Ignored characters
    case ' ':
    case '\r':
    case '\t':
        break;

    // Newline characters
    case '\n':
        currentLine++;
        currentColumn = 0;
        break;

    case '"':

        string();
        break;

    // Numeric literals
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        number();
        break;

    // Identifiers
    default:
        if (isAlpha(c))
        {
            identifier();
        }
        else
        {
            addToken(ERROR, "Unexpected character");
        }
        break;
    }
}

// Check if a character is a digit
bool Lexer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

// Check if a character is a letter
bool Lexer::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// Check if a character is alphanumeric
bool Lexer::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

// Move the current character pointer to the next character
char Lexer::advance()
{
    currentColumn++;
    return input[current++];
}

// Get the next character without moving the pointer
char Lexer::peek()
{
    if (current >= input.length())
    {
        return '\0';
    }
    return input[current];
}

// Get the character after the next character without moving the pointer
char Lexer::peekNext()
{
    if (current + 1 >= input.length())
    {
        return '\0';
    }
    return input[current + 1];
}

// Check if the next character matches the expected character
bool Lexer::match(char expected)
{
    if (peek() == expected)
    {
        advance();
        return true;
    }
    return false;
}

void Lexer::number()
{
    while (isDigit(peek()))
    {
        advance();
    }
    // Look for a decimal point
    if (peek() == '.' && isDigit(peekNext()))
    {
        advance(); // Consume the decimal point

        while (isDigit(peek()))
        {
            advance();
        }
    }

    if (peek() == 'f' || peek() == 'F')
    {
        advance();
        addToken(FLOAT, input.substr(start, current - start));
    }
    else
    {
        addToken(INT, input.substr(start, current - start));
    }
}

void Lexer::identifier()
{
    while (isAlphaNumeric(peek()))
    {
        advance();
    }
    std::string text = input.substr(start, current - start);

    if (text == "int")
    {
        addToken(INT, text);
    }
    else if (text == "float")
    {
        addToken(FLOAT, text);
    }
    else if (text == "char")
    {
        addToken(CHAR, text);
    }
    else if (text == "string")
    {
        addToken(STRING, text);
    }
    else if (text == "main")
    {
        addToken(MAIN, text);
    }
    else
    {
        addToken(IDENTIFIER, text);
    }
}

// Parse a string literal
void Lexer::string()
{
    while (peek() != '"' && peek() != '\0')
    {
        if (peek() == '\\' && peekNext() == '\"')
        {
            advance(); // Consume the ''
            advance(); // Consume the '"'
        }
        else
        {
            if (peek() == '\n')
            {
                currentLine++;
                currentColumn = 1;
            }
            advance();
        }
    }
    if (peek() == '\0')
    {
        addToken(ERROR, "Unterminated string");
    }
    else
    {
        advance();                                                      // Consume the closing quote
        addToken(STRING, input.substr(start + 1, current - start - 2)); // Trim the surrounding quotes
    }
}

// Get the list of tokens

// Print the list of tokens
void Lexer::printTokens()
{
    for (Token token : tokens)
    {

        TokenType token_type = static_cast<TokenType>(token.type);

        cout << mapping[token_type] << " " << token.lexeme << " " << token.line << endl;
    }
}

void Lexer :: outputToFile()
{
    std::ofstream outFile("output.txt");
    // the important part
    for (const auto &e : tokens) outFile << mapping[e.type] <<" "<<e.lexeme<<" "<<e.line<< "\n";


}

// Main function to test the lexer
int main()
{
    // Sample input program
    string input = "program.txt";
    Lexer lexer(input);
    lexer.getTokens();
    lexer.printTokens();
    lexer.outputToFile();

    return 0;
}