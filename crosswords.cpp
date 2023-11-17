#include <iostream>
#include <cassert>
#include "crosswords.h"

// __________GLOBAL VARIABLES__________

constinit char DEFAULT_CHAR = '?';
constinit std::string_view DEFAULT_WORD{"?"};
constinit char CROSSWORD_BACKGROUND = '.';
constexpr int ENLARGE = 'A' - 'a';


// __________RectArea CLASS IMPLEMENTATION__________

// Constructors:

RectArea::RectArea(const pos_t& lt, const pos_t& rb) :
        left_top(lt),
        right_bottom(rb) {
}

RectArea::RectArea(const RectArea& rect_area) :
        left_top(rect_area.left_top),
        right_bottom(rect_area.right_bottom) {
}

RectArea::RectArea(RectArea&& rect_area) :
        left_top(std::move(rect_area.left_top)),
        right_bottom(std::move(rect_area.right_bottom)) {
}


// Operators:

RectArea& RectArea::operator=(const RectArea& rect_area) {
    left_top = rect_area.left_top;
    right_bottom = rect_area.right_bottom;
    return *this;
}

RectArea& RectArea::operator=(RectArea&& rect_area) {
    left_top = std::move(rect_area.left_top);
    right_bottom = std::move(rect_area.right_bottom);
    return *this;
}

RectArea operator*(const RectArea& rect_area1, const RectArea& rect_area2) {
    size_t resLT_x = std::max(rect_area1.left_top.first,
                              rect_area2.left_top.first);
    size_t resLT_y = std::max(rect_area1.left_top.second,
                              rect_area2.left_top.second);
    size_t resRB_x = std::min(rect_area1.right_bottom.first,
                              rect_area2.right_bottom.first);
    size_t resRB_y = std::min(rect_area1.right_bottom.second,
                              rect_area2.right_bottom.second);

    if ((resLT_x > resRB_x) || (resLT_y > resRB_y))
        return DEFAULT_EMPTY_RECT_AREA;

    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    return RectArea{resLT, resRB};
}

RectArea& operator*=(RectArea& rect_area1, const RectArea& rect_area2) {
    size_t resLT_x = std::max(rect_area1.left_top.first,
                              rect_area2.left_top.first);
    size_t resLT_y = std::max(rect_area1.left_top.second,
                              rect_area2.left_top.second);
    size_t resRB_x = std::min(rect_area1.right_bottom.first,
                              rect_area2.right_bottom.first);
    size_t resRB_y = std::min(rect_area1.right_bottom.second,
                              rect_area2.right_bottom.second);

    if ((resLT_x > resRB_x) || (resLT_y > resRB_y)) {
        rect_area1 = DEFAULT_EMPTY_RECT_AREA;
        return rect_area1;
    }

    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    rect_area1 = RectArea{resLT, resRB};
    return rect_area1;
}


// Public functions:

pos_t RectArea::get_left_top() const {
    return left_top;
}

void RectArea::set_left_top(const pos_t& new_pos) {
    left_top = new_pos;
}

pos_t RectArea::get_right_bottom() const {
    return right_bottom;
}

void RectArea::set_right_bottom(const pos_t& new_pos) {
    right_bottom = new_pos;
}

dim_t RectArea::size() const {
    if (right_bottom.first < left_top.first)
        return {0,0};

    if (right_bottom.second < left_top.second)
        return {0,0};

   assert(right_bottom.first - left_top.first <= SIZE_MAX);
   assert(right_bottom.second - left_top.second <= SIZE_MAX);

    return {right_bottom.first - left_top.first + 1,
            right_bottom.second - left_top.second + 1};
}

bool RectArea::empty() const {
    return size() == dim_t{0, 0};
}

void RectArea::embrace(pos_t pos) {
    if (this->empty()) {
        left_top = pos;
        right_bottom = pos;
        return;
    }
    set_left_top(pos_t(std::min(left_top.first, pos.first),
                       std::min(left_top.second, pos.second)));
    set_right_bottom(pos_t(std::max(right_bottom.first, pos.first),
                           std::max(right_bottom.second, pos.second)));
}


// __________Word CLASS IMPLEMENTATION__________

// Constructors:

Word::Word(size_t x, size_t y, orientation_t o, const std::string& s) {
    orientation = o;
    position = pos_t(std::pair(x, y));
    if (s.empty()) str = DEFAULT_WORD;
    else str = s;
}

Word::Word(const Word &word) :
        position(word.position),
        orientation(word.orientation),
        str(word.str) {
}

Word::Word(Word &&word) :
        position(std::move(word.position)),
        orientation(word.orientation),
        str(std::move(word.str)) {
}


// Operators:

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
    if (position.first < word.position.first)
        return std::strong_ordering::less;

    if (position.first > word.position.first)
        return std::strong_ordering::greater;

    if (position.second < word.position.second)
        return std::strong_ordering::less;

    if (position.second < word.position.second)
        return std::strong_ordering::greater;

    if (orientation < word.orientation)
        return std::strong_ordering::less;

    if (orientation > word.orientation)
        return std::strong_ordering::greater;

    return std::strong_ordering::equal;
}


// Public functions:

pos_t Word::get_start_position() const {
    return position;
}

pos_t Word::get_end_position() const {
    pos_t end = position;
    size_t increment =  str.size() - 1;
    if (orientation == orientation_t::H) {

        if (SIZE_MAX - end.first >= increment)
        end.first += increment;
        else {
            end.first = SIZE_MAX;
        }
    }
    else  {
        if (SIZE_MAX - end.second >= increment) {
            end.second += increment;
        }
        else {
            end.second = SIZE_MAX;
        }
    }
    return end;
}

orientation_t  Word::get_orientation() const {
    return orientation;
}

std::string Word::get_str() const {
    return str;
}

size_t Word::length() const {
    return str.size();
}

char Word::at(size_t ind) const {
    if (ind >= str.size())
        return DEFAULT_CHAR;

    return str.at(ind);
}

RectArea Word::rect_area() const {
    return RectArea{get_start_position(), get_end_position()};
}

RectArea Word::get_rect_area_with_frame() const {
    RectArea frame = rect_area();
    pos_t frameBeg, frameEnd;
    pos_t begPos = get_start_position();
    pos_t endPos = get_end_position();

    frameBeg = std::pair(begPos.first == 0 ? 0 : begPos.first - 1 ,
                         (begPos.second == 0 ? 0 : begPos.second - 1));
    frameEnd = std::pair( endPos.first + 1,endPos.second + 1);
    frame.embrace(frameBeg);
    frame.embrace(frameEnd);
    return frame;
}


// __________Crossword CLASS IMPLEMENTATION__________

// Constructors:

Crossword::Crossword(const Word& w, std::initializer_list<Word> wordList) :
        rect_area(w.rect_area()) {

    insert_word(w);
    for (auto i = wordList.begin(); i < wordList.end(); i++)
        insert_word(*i);
}

Crossword::Crossword(const Crossword& crossword) :
        words(crossword.words),
        countV(crossword.countV),
        countH(crossword.countH),
        rect_area(crossword.rect_area),
        fullArea(crossword.fullArea) {

    if (crossword.countH + crossword.countV == 0)
        rect_area = DEFAULT_EMPTY_RECT_AREA;
}

Crossword::Crossword(Crossword&& crossword) :
        words(std::move(crossword.words)),
        countV(crossword.countV),
        countH(crossword.countH),
        rect_area(std::move(crossword.rect_area)),
        fullArea(std::move(crossword.fullArea)) {

   crossword.countH = 0;
    crossword.countV = 0;
    crossword.rect_area = DEFAULT_EMPTY_RECT_AREA;
    crossword.words = {};
    crossword.fullArea = {};
}


// Operators:

Crossword& Crossword::operator=(const Crossword& cr) {
    words = cr.words;
    rect_area = cr.rect_area;
    countV = cr.countV;
    countH = cr.countH;
    fullArea = cr.fullArea;
    return *this;
}

Crossword& Crossword::operator=(Crossword&& cr) {
    words = std::move(cr.words);
    rect_area = std::move(cr.rect_area);
    countV = cr.countV;
    countH = cr.countH;
    fullArea = std::move(cr.fullArea);
    return *this;
}

Crossword Crossword::operator+(const Crossword& cr) {
    Crossword newCross{*this};
    for (const auto& w : cr.words)
        newCross.insert_word(w);

    return newCross;
}

Crossword& Crossword::operator+=(const Crossword& cr) {
    for (const auto& w : cr.words)
        (*this).insert_word(w);

    return *this;
}


// Private functions:

pos_t Crossword::nextPos(const pos_t& pos, orientation_t orientation) {
    pos_t res = pos;
    if (orientation == orientation_t::H) ++res.first;
    else ++res.second;

    return res;
}

void Crossword::correctLetter(char &c) {
    if (c >= 'a' && c <= 'z') c += ENLARGE;     // do funkcji???
    else if (c < 'A' || c > 'Z') c = DEFAULT_CHAR;
}

bool Crossword::collision(const Word& word) {
    RectArea frame = word.get_rect_area_with_frame();

    for (const Word& w : words) {
        RectArea product = w.rect_area() * frame;
        if (!product.empty()) {
            if (word.get_orientation() == w.get_orientation()) return true;
            else {
                RectArea wordProduct = w.rect_area() * word.rect_area();
                if (wordProduct.empty()) return true;

                pos_t wordProdPos = wordProduct.get_left_top();
                pos_t wordBegPos = word.get_start_position();
                size_t idx = std::max(wordProdPos.first - wordBegPos.first,
                                      wordProdPos.second - wordBegPos.second);

                char toCheck = word.at(idx);     // at?
                correctLetter(toCheck);

                if (fullArea.contains(wordProdPos)
                    && fullArea[wordProdPos] != toCheck) return true;
            }
        }
    }
    return false;
}


// Public functions:

bool Crossword::insert_word(const Word& word) {
    if (collision(word))
        return false;

    orientation_t orientation = word.get_orientation();
    pos_t begPos = word.get_start_position();
    pos_t endPos = word.get_end_position();

    if (word.get_orientation() == orientation_t::H) ++countH;
    else ++countV;

    words.insert(word);
    rect_area.embrace(begPos);
    rect_area.embrace(endPos);

    size_t idx = 0;
    while (begPos <= endPos) {
        char toAdd = word.at(idx);  // było get letter
        correctLetter(toAdd);

        fullArea.insert({begPos, toAdd});
        if (begPos == endPos) {
            break;
        }
        begPos = nextPos(begPos, orientation);
        idx++;

    }
    return true;
}

dim_t Crossword::size() const{
    return rect_area.size();
}

count_t Crossword::word_count() const{
    return {countH, countV};
}

// printing:
std::ostream& operator<<(std::ostream& out, const Crossword& cross) {
    pos_t pos = cross.rect_area.get_left_top();
    pos_t end = cross.rect_area.get_right_bottom();

    out << CROSSWORD_BACKGROUND << ' ';
    for (auto j = pos.first; j <= end.first; j++) {
        out << CROSSWORD_BACKGROUND << ' ';
    }
    out << CROSSWORD_BACKGROUND << '\n';
    for (auto i = pos.second; i <= end.second + 1; i++) {  //cast na cos nie-unsigned???????????? albo wyrzucic do petli wypisywanie pierwszej linii
        out << CROSSWORD_BACKGROUND << ' ';
        for (auto j = pos.first; j <= end.first; j++) {
            if (cross.fullArea.contains({j, i})) {
                char c = cross.fullArea.at({j, i});
                cross.correctLetter(c);
                out << c;
            }
            else out << CROSSWORD_BACKGROUND;

            out << ' ';
        }
        out << CROSSWORD_BACKGROUND << '\n';
    }

    return out;
}
