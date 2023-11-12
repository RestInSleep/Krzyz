#include "crosswords.h"
#include <iostream>
#include <list>

// __________GLOBAL VARIABLES__________

constinit char DEFAULT_NON_LETTER = '?';
const std::string DEFAULT_STRING = "?"; // TODO: constinit ????
constinit char DEFAULT_BACKGROUND = '.';

constinit pos_t START_POS = std::pair(0, 0);
const RectArea EMPTY_AREA = RectArea(START_POS, START_POS); // TODO: constinit ????
constinit int ENLARGE = 'A' - 'a';


// __________Word CLASS IMPLEMENTATION__________

Word::Word(size_t x, size_t y, orient_t o, const std::string& s) {
    orientation = o;
    position = pos_t(std::pair(x, y));
    if (s.empty()) {
        str = DEFAULT_STRING;
    }
    else {
        str = s;
    }
}

Word::Word(const Word &word) :
        position(word.position),
        orientation(word.orientation),
        str(word.str) {}

Word::Word(Word &&word) :
        position(std::move(word.position)),
        orientation(word.orientation),
        str(std::move(word.str)) {}

// TODO: konstruktory

pos_t Word::getPosBegin() const {
    return position;
}

pos_t Word::getPosEnd() const {
    pos_t end = position;
    if (orientation == orientation_t::H) end.second += str.size() - 1;
    else end.first += str.size() - 1;
    return end;
}

orientation_t  Word::getOrientation() const {
    return orientation;
}

char Word::getLetter(size_t idx) const {
    return str[idx];
}
std::string Word::getStr() const {
return str;
}

size_t Word::getSize() const {
    return str.size();
}

RectArea Word::getRectArea() const {
    return {getPosBegin(), getPosEnd()};
}
//
////TODO: przypisanie
Word& Word::operator=(const Word& word) {
    position = word.position;
    orientation = word.orientation;
    str = word.str;
    return *this;
}

Word& Word::operator=(Word&& word) {
    position = std::move(word.position);
    orientation = word.orientation;
    str = std::move(word.str);
    return *this;
}

bool Word::operator==(const Word& word) const {
    return (position.first == word.position.first &&
            position.second == word.position.second &&
            orientation == word.orientation);
}
bool Word::operator!=(const Word& word) const {
    return !(*this == word);
}



std::strong_ordering Word::operator<=>(const Word& word) const {
    if (position.first < word.position.first) {
        return std::strong_ordering::less;
    }
    if (position.first > word.position.first) {
        return std::strong_ordering::greater;
    }
    if (position.second < word.position.second) {
        return std::strong_ordering::less;
    }
    if (position.second < word.position.second) {
        return std::strong_ordering::greater;
    }
    if (orientation < word.orientation) {
        return std::strong_ordering::less;
    }
    if (orientation > word.orientation) {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
}


// __________RectArea CLASS IMPLEMENTATION__________

RectArea::RectArea(const pos_t& lt, const pos_t& rb): leftTop(lt),
                                                        rightBottom(rb){}

RectArea::RectArea(const RectArea& rectArea): leftTop(rectArea.leftTop),
rightBottom(rectArea.rightBottom)
{}

RectArea::RectArea(RectArea&& rectArea): leftTop(std::move(rectArea.leftTop)),
rightBottom(std::move(rectArea.rightBottom))
{}

RectArea& RectArea::operator=(const RectArea& rectArea){
    leftTop = rectArea.leftTop;
    rightBottom = rectArea.rightBottom;
    return *this;
}
RectArea& RectArea::operator=(RectArea&& rectArea){
    leftTop = std::move(rectArea.leftTop);
    rightBottom = std::move(rectArea.rightBottom);
    return *this;
}

RectArea operator*(const RectArea& rectArea1, const RectArea& rectArea2) {

    size_t resLT_x = std::max(rectArea1.leftTop.first, rectArea2.leftTop.first);
    size_t resLT_y = std::min(rectArea1.leftTop.second, rectArea2.leftTop.second);
    size_t resRB_x = std::max(rectArea1.rightBottom.first, rectArea2.rightBottom.first);
    size_t resRB_y = std::min(rectArea1.rightBottom.second, rectArea2.rightBottom.second);
    if ((resLT_x >= resRB_x) || (resLT_y <= resRB_y)) {
        return EMPTY_AREA;
    }
    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    return RectArea{resLT, resRB};
}

RectArea& operator*=(RectArea& rectArea1, const RectArea& rectArea2) {

    size_t resLT_x = std::max(rectArea1.leftTop.first, rectArea2.leftTop.first);
    size_t resLT_y = std::min(rectArea1.leftTop.second, rectArea2.leftTop.second);
    size_t resRB_x = std::max(rectArea1.rightBottom.first, rectArea2.rightBottom.first);
    size_t resRB_y = std::min(rectArea1.rightBottom.second, rectArea2.rightBottom.second);
    if ((resLT_x > resRB_x) || (resLT_y < resRB_y)) {
        rectArea1 = EMPTY_AREA;
        return rectArea1;
    }
    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    rectArea1 = RectArea{resLT, resRB};
    return rectArea1;
}



// TODO: konstruktory, operatory

pos_t RectArea::getLeftTop() const {
    return leftTop;
}

void RectArea::setLeftTop(const pos_t& newPos) {
    leftTop = newPos;
}

pos_t RectArea::getRightBottom() const {
    return rightBottom;
}

void RectArea::setRightBottom(const pos_t& newPos) {
    rightBottom = newPos;
}

dim_t RectArea::getDim() const {
    return dim_t{std::max((size_t) 0, rightBottom.first - leftTop.first + 1),
                 std::max((size_t) 0, rightBottom.second - leftTop.second + 1)};
}

bool RectArea::isEmpty() {
    return getDim() == dim_t(0, 0);
}

void RectArea::resize(pos_t pos) {

    setLeftTop(pos_t(std::min(leftTop.first, pos.first),
                     std::min(leftTop.second, pos.second)));
    setRightBottom(pos_t(std::max(rightBottom.first, pos.first),
                         std::max(rightBottom.second, pos.second)));
}


// __________Crossword CLASS IMPLEMENTATION__________



Crossword::Crossword(Word w, std::initializer_list<Word> wordList) :
        rectArea(RectArea(START_POS, START_POS)){
    insert(w);
    for (auto i = wordList.begin(); i < wordList.end(); i++) insert((Word&) i); // ???
}

// TODO: konstruktory

pos_t Crossword::nextPos(pos_t pos, orient_t orientation) {
    pos_t res = pos;
    if(orientation == orient_t::H) res.first++;
    else res.second++;
    return res;
}

bool Crossword::collision(Word word) {
    pos_t pos = word.getPosBegin();
    size_t idx = 0;
    orient_t orientation = word.getOrientation();
    while(pos <= word.getPosEnd()){
        auto letter = fullArea[pos];
        if(fullArea.contains(pos) && fullArea[pos] != word.getLetter(idx))
            return false;
        pos = nextPos(pos, orientation);
        idx++;
    }
    return true;
}

void Crossword::insert(Word word) {
    if(!collision(word)){
        if (word.getOrientation() == orient_t::H) countH++;
        else countV++;

        words.insert(&word);

        rectArea.resize(word.getPosBegin());
        rectArea.resize(word.getPosEnd());

        pos_t pos = word.getPosBegin();
        size_t idx = 0;
        orient_t orientation = word.getOrientation();
        while(pos <= word.getPosEnd()){
            fullArea.insert(std::pair(pos, word.getLetter(idx)));
            pos = nextPos(pos, orientation);
            idx++;
        }
    }
}

dim_t Crossword::getDim() {
    return rectArea.getDim();
}

count_t Crossword::getCount() {
    return count_t(countH, countV);
}

void Crossword::print(char background) {
    pos_t pos = rectArea.getLeftTop();
    pos_t end = rectArea.getRightBottom();
    for(auto i = pos.first; i <= end.first; i++){
        for (auto j = pos.second; j <= end.second; j++){
            if(fullArea.contains(pos)) {
                char c = fullArea[pos];
                if (c >= 'a' && c <= 'z') c += ENLARGE;
                else if (c < 'A' && c > 'Z') c = DEFAULT_NON_LETTER;
                std::cout << c << " ";
            }
            else {
                std::cout << background << " ";
            }
            pos = nextPos(pos, orient_t::V);
        }
        pos = nextPos(pos, orient_t::H);
    }
}

