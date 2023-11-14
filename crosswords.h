#ifndef CROSSWORDS
#define CROSSWORDS

#include <iostream>
#include <set>
#include <map>

static constinit char DEFAULT_NON_LETTER = '?';
static const std::string DEFAULT_STRING = "?"; // TODO: constinit ????
static constinit char DEFAULT_BACKGROUND = '.';



using pos_t = std::pair<size_t, size_t>;
using dim_t = std::pair<size_t, size_t>;
using count_t = std::pair<size_t, size_t>; // size_t ???
enum orientation_t : bool {H, V};

class RectArea {
private:
    pos_t leftTop;
    pos_t rightBottom;

public:
    RectArea(const pos_t& lt, const pos_t& rb);
    RectArea(const pos_t& rb) = delete;
    RectArea() = delete;
    RectArea(const RectArea &rectArea);
    RectArea(RectArea &&rectArea);

    RectArea& operator=(const RectArea&);
    RectArea& operator=(RectArea&&);

    pos_t getLeftTop() const;
    void setLeftTop(const pos_t& newPos);
    pos_t getRightBottom() const;
    void setRightBottom(const pos_t& newPos);

    dim_t getDim() const;
    bool isEmpty() const;
    void resize(pos_t pos);
    friend RectArea operator*(const RectArea& rectArea1, const RectArea& rectArea2);
    friend RectArea& operator*=(RectArea& rectArea1, const RectArea& rectArea2);

    //TODO: operatory, operacje przypisania
};

class Word {
private:
    pos_t position;
    orientation_t orientation;
    std::string str;

public:
    Word(size_t x, size_t y, orientation_t o, const std::string& s);
    Word(const Word &word);
    Word(Word &&word);

    char at(unsigned int ind) const;

    pos_t getPosBegin() const;
    pos_t getPosEnd() const;
    orientation_t getOrientation () const;
    char getLetter(size_t idx) const;
    size_t getSize() const;
    RectArea getRectArea() const;
    std::string getStr() const;
    RectArea getRectAreaWithFrame() const;
    //TODO: operatory, operacje przypisania


    std::strong_ordering operator<=>(const Word& word) const;

    Word& operator=(const Word& word);
    Word& operator=(Word&& word);
    bool operator==(const Word& word) const;
    bool operator!=(const Word& word) const;
};

class Crossword {
private:
    std::set<Word> words;
    size_t countV = 0;
    size_t countH = 0;
    RectArea rectArea;
    std::map<pos_t, char> fullArea;

    bool emptyBeforeAfter(const Word& word);
    static pos_t nextPos(const pos_t& pos, orientation_t orientation);
    bool collision(const Word& word);

public:
    Crossword(const Word& w, std::initializer_list<Word> wordList);
    Crossword(const Crossword& crossword);
    Crossword(Crossword&& crossword);
    Crossword& operator+=(const Crossword& cr);
    Crossword& operator=(const Crossword&);
    Crossword& operator=(Crossword&&);

    Crossword operator+(const Crossword&);


    bool insert(const Word& word);
    dim_t getDim();
    count_t getCount();

    void print(char background = DEFAULT_BACKGROUND);
    //TODO: operatory, operacje przypisania
};

static constinit pos_t START_POS = std::pair(0, 0);
static const RectArea EMPTY_AREA = RectArea{{1,1}, {0,0}}; // TODO: constinit ????
static constinit int ENLARGE = 'A' - 'a';

#endif