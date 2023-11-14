#include "crosswords.h"
#include <iostream>
#include <list>

// __________GLOBAL VARIABLES__________




// __________Word CLASS IMPLEMENTATION__________

Word::Word(size_t x, size_t y, orientation_t o, const std::string& s) {
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
    if (orientation == orientation_t::H) {
        end.first += str.size() - 1;
    }
    else {
        end.second += str.size() - 1;
    }
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
    return RectArea{getPosBegin(), getPosEnd()};
}

char Word::at(unsigned int ind) const {
    if(ind >= str.size())
        return DEFAULT_NON_LETTER;
    return str.at(ind);
};
RectArea Word::getRectAreaWithFrame() const {
    RectArea tmp = getRectArea();
    pos_t tmpBeg, tmpEnd;
    if (getOrientation() == H) {
        tmpBeg = std::pair(getPosBegin().first,
                           (getPosBegin().second == 0 ? 0 : getPosBegin().second - 1));
        tmpEnd = std::pair(getPosEnd().first,
                           getPosEnd().second + 1);
    }
    else {
        tmpBeg = std::pair((getPosBegin().first == 0 ? 0 : getPosBegin().first - 1),
                           getPosBegin().second);
        tmpEnd = std::pair(getPosEnd().first + 1,
                           getPosEnd().second);
    }
    tmp.resize(tmpBeg);
    tmp.resize(tmpEnd);
    return tmp;
}

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

RectArea::RectArea(const pos_t& lt, const pos_t& rb)
        :
        leftTop(lt),
        rightBottom(rb)
{}

RectArea::RectArea(const RectArea& rectArea)
        :
        leftTop(rectArea.leftTop),
        rightBottom(rectArea.rightBottom)
{}

RectArea::RectArea(RectArea&& rectArea)
        :
        leftTop(std::move(rectArea.leftTop)),
        rightBottom(std::move(rectArea.rightBottom))
{}

RectArea& RectArea::operator=(const RectArea& rectArea) {
    leftTop = rectArea.leftTop;
    rightBottom = rectArea.rightBottom;
    return *this;
}

RectArea& RectArea::operator=(RectArea&& rectArea) {
    leftTop = std::move(rectArea.leftTop);
    rightBottom = std::move(rectArea.rightBottom);
    return *this;
}

RectArea operator*(const RectArea& rectArea1, const RectArea& rectArea2) {

    size_t resLT_x = std::max(rectArea1.leftTop.first, rectArea2.leftTop.first);
    size_t resLT_y = std::max(rectArea1.leftTop.second, rectArea2.leftTop.second);
    size_t resRB_x = std::min(rectArea1.rightBottom.first, rectArea2.rightBottom.first);
    size_t resRB_y = std::min(rectArea1.rightBottom.second, rectArea2.rightBottom.second);
    if ((resLT_x > resRB_x) || (resLT_y > resRB_y)) {
        return EMPTY_AREA;
    }
    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    return RectArea{resLT, resRB};
}

RectArea& operator*=(RectArea& rectArea1, const RectArea& rectArea2) {

    size_t resLT_x = std::max(rectArea1.leftTop.first, rectArea2.leftTop.first);
    size_t resLT_y = std::max(rectArea1.leftTop.second, rectArea2.leftTop.second);
    size_t resRB_x = std::min(rectArea1.rightBottom.first, rectArea2.rightBottom.first);
    size_t resRB_y = std::min(rectArea1.rightBottom.second, rectArea2.rightBottom.second);
    if ((resLT_x > resRB_x) || (resLT_y > resRB_y)) {
        rectArea1 = EMPTY_AREA;
        return rectArea1;
    }
    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    rectArea1 = RectArea{resLT, resRB};
    return rectArea1;
}


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
    if (rightBottom.first < leftTop.first) {
        return {0,0};
    }
    if (rightBottom.second < leftTop.second) {
        return {0,0};
    }
    return {rightBottom.first - leftTop.first  + 1, rightBottom.second -
                                                    leftTop.second + 1};

    //    return dim_t{
//        std::max((size_t) 0, rightBottom.first - leftTop.first),
//                 std::max((size_t) 0, rightBottom.second - leftTop.second)};
}

bool RectArea::isEmpty() const {
    return getDim() == dim_t{0, 0};
}

void RectArea::resize(pos_t pos) {

    if(this->isEmpty()) {
        leftTop = pos;
        rightBottom = pos;
        return;
    }
    setLeftTop(pos_t(std::min(leftTop.first, pos.first),
                     std::min(leftTop.second, pos.second)));
    setRightBottom(pos_t(std::max(rightBottom.first, pos.first),
                         std::max(rightBottom.second, pos.second)));
}


// __________Crossword CLASS IMPLEMENTATION__________

Crossword::Crossword(const Word& w, std::initializer_list<Word> wordList) :
        rectArea(w.getRectArea()){
    insert(w);
    for (auto i = wordList.begin(); i < wordList.end(); i++) {
        insert(*i);
    }

}

Crossword::Crossword(const Crossword& crossword)
        :
        countH(crossword.countH),
        countV(crossword.countV),
        words(crossword.words),
        rectArea(crossword.rectArea),
        fullArea(crossword.fullArea)
{
    if (crossword.countH + crossword.countV == 0) {
        rectArea = EMPTY_AREA;
    }
}

Crossword::Crossword(Crossword&& crossword)
        :
        countH(crossword.countH),
        countV(crossword.countV),
        words(std::move(crossword.words)),
        rectArea(std::move(crossword.rectArea)),
        fullArea(std::move(crossword.fullArea))
{
    if (crossword.countH + crossword.countV == 0) {
        rectArea = EMPTY_AREA;
    }
}

Crossword Crossword::operator+(const Crossword& cr){
   Crossword newCross{*this};
    for (const auto& w : cr.words) {
        newCross.insert(w);
    }
    return newCross;
}



Crossword& Crossword::operator+=(const Crossword& cr){
    for (const auto& w : cr.words){
        (*this).insert(w);
    }
    return *this;
}

Crossword& Crossword::operator=(const Crossword& cr){
    words = cr.words;
    rectArea = cr.rectArea;
    countV = cr.countV;
    countH = cr.countH;
    fullArea = cr.fullArea;
    return *this;
}

Crossword& Crossword::operator=(Crossword&& cr){
    words = std::move(cr.words);
    rectArea = std::move(cr.rectArea);
    countV = cr.countV;
    countH = cr.countH;
    fullArea = std::move(cr.fullArea);
    return *this;
}

pos_t Crossword::nextPos(const pos_t& pos, orientation_t orientation) {
    pos_t res = pos;
    if(orientation == orientation_t::H)  {
        ++res.first;
    }
    else {
        ++res.second;
    }
    return res;
}

bool Crossword::emptyBeforeAfter(const Word& word) {
    int8_t col = 0, row = 0;
    if (word.getOrientation() == H) {
        ++col;
    }
    else {
        ++row;
    }
    if (word.getPosBegin().first != 0) {
        pos_t before = std::pair(word.getPosBegin().first - col, word.getPosBegin().second - row);
        if (fullArea.contains(before)) {
            return false;
        }
    }
    pos_t after = std::pair(word.getPosEnd().first + col, word.getPosEnd().second + row);
    if(fullArea.contains(after)) {
        return false;
    }

    return true;
}

bool Crossword::collision(const Word& word) {
    //kratka przed i po:
    if(!emptyBeforeAfter(word)) return true;

    //RectArea z ramką
    RectArea frame = word.getRectAreaWithFrame();

    for (const Word& w : words) {
        dim_t dim = (w.getRectArea() * frame).getDim(); // wymiary czesci wspolnej
        size_t area = dim.first * dim.second; // pole powierzchni czesci wspolnej (liczba kratek)
        if(w.getOrientation() == word.getOrientation()) {
            if(area != 0) return true;
        }
        else {
            if(area != 0 && area != 2 && area != 3 && w.getSize() != 0) return true;
            else {
                if (area > 0) {


                    pos_t pos = (w.getRectArea() * word.getRectArea()).getLeftTop();
                    size_t idx = std::max(pos.first - word.getPosBegin().first,
                                          pos.second - word.getPosBegin().second);

                    char toCheck = word.getLetter(idx);
                    if (toCheck >= 'a' && toCheck <= 'z') {
                        toCheck += ENLARGE;
                    }
                    if (fullArea.contains(pos) && fullArea[pos] != toCheck)
                        return true;
                }
            }
        }
    }
    return false;
}

bool Crossword::insert(const Word& word) {
    if (collision(word))
    return false;

        if (word.getOrientation() == orientation_t::H) ++countH;
        else ++countV;

        words.insert(word);

        rectArea.resize(word.getPosBegin());
        rectArea.resize(word.getPosEnd());

        pos_t pos = word.getPosBegin();
        size_t idx = 0;
        orientation_t orientation = word.getOrientation();
        while(pos <= word.getPosEnd()){
            //TODO: ogarnac male i durze
            char toAdd = word.getLetter(idx);
            if (toAdd >= 'a' && toAdd <= 'z') {
                toAdd += ENLARGE;
            }
            fullArea.insert({pos, toAdd});
            pos = nextPos(pos, orientation);
            idx++;
        }
    return true;
}

dim_t Crossword::getDim() {
    return rectArea.getDim();
}

count_t Crossword::getCount() {
    return {countH, countV};
}

void Crossword::print(char background) {
    pos_t pos = rectArea.getLeftTop();
    pos_t end = rectArea.getRightBottom();
    for (auto i = pos.second; i <= end.second; i++) {
        for (auto j = pos.first; j <= end.first; j++){
            if (fullArea.contains({j,i})) {
                char c = fullArea[{j,i}];
                if (c >= 'a' && c <= 'z') c += ENLARGE;
                else if (c < 'A' || c > 'Z') c = DEFAULT_NON_LETTER;
                std::cout << c << " ";
            }
            else {
                std::cout << background << " ";
            }

        }
        std::cout << '\n';
    }




}
