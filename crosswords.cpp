
#include <iostream>
#include "crosswords.h"

// __________GLOBAL VARIABLES__________
constinit char DEFAULT_CHAR = '?';
constinit std::string_view DEFAULT_WORD{"?"}; // TODO: constinit ????
constinit char CROSSWORD_BACKGROUND = '.';
constexpr int ENLARGE = 'A' - 'a';


// __________Word CLASS IMPLEMENTATION__________

Word::Word(size_t x, size_t y, orientation_t o, const std::string& s) {
    orientation = o;
    position = pos_t(std::pair(x, y));
    if (s.empty()) {
        str = DEFAULT_WORD;
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

pos_t Word::get_start_position() const {
    return position;
}

pos_t Word::get_end_position() const {
    pos_t end = position;
    if (orientation == orientation_t::H) {
        end.first += str.size() - 1;
    }
    else {
        end.second += str.size() - 1;
    }
    return end;
}

orientation_t  Word::get_orientation() const {
    return orientation;
}

char Word::getLetter(size_t idx) const {
    return str[idx];
}
std::string Word::get_str() const {
    return str;
}

size_t Word::length() const {
    return str.size();
}

RectArea Word::rect_area() const {
    return RectArea{get_start_position(), get_end_position()};
}

char Word::at(unsigned int ind) const {
    if(ind >= str.size())
        return DEFAULT_CHAR;
    return str.at(ind);
}

RectArea Word::get_rect_area_with_frame() const {
    RectArea tmp = rect_area();
    pos_t tmpBeg, tmpEnd;

    tmpBeg = std::pair(get_start_position().first == 0 ? 0 : get_start_position().first - 1 ,
                       (get_start_position().second == 0 ? 0 : get_start_position().second - 1));
    tmpEnd = std::pair( get_end_position().first + 1,get_end_position().second + 1);
    tmp.embrace(tmpBeg);
    tmp.embrace(tmpEnd);
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
        left_top(lt),
        right_bottom(rb)
{}

RectArea::RectArea(const RectArea& rect_area)
        :
        left_top(rect_area.left_top),
        right_bottom(rect_area.right_bottom)
{}

RectArea::RectArea(RectArea&& rect_area)
        :
        left_top(std::move(rect_area.left_top)),
        right_bottom(std::move(rect_area.right_bottom))
{}



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

    size_t resLT_x = std::max(rect_area1.left_top.first, rect_area2.left_top.first);
    size_t resLT_y = std::max(rect_area1.left_top.second, rect_area2.left_top.second);
    size_t resRB_x = std::min(rect_area1.right_bottom.first, rect_area2.right_bottom.first);
    size_t resRB_y = std::min(rect_area1.right_bottom.second, rect_area2.right_bottom.second);
    if ((resLT_x > resRB_x) || (resLT_y > resRB_y)) {
        return DEFAULT_EMPTY_RECT_AREA;
    }
    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    return RectArea{resLT, resRB};
}

RectArea& operator*=(RectArea& rect_area1, const RectArea& rect_area2) {

    size_t resLT_x = std::max(rect_area1.left_top.first, rect_area2.left_top.first);
    size_t resLT_y = std::max(rect_area1.left_top.second, rect_area2.left_top.second);
    size_t resRB_x = std::min(rect_area1.right_bottom.first, rect_area2.right_bottom.first);
    size_t resRB_y = std::min(rect_area1.right_bottom.second, rect_area2.right_bottom.second);
    if ((resLT_x > resRB_x) || (resLT_y > resRB_y)) {
        rect_area1 = DEFAULT_EMPTY_RECT_AREA;
        return rect_area1;
    }
    pos_t resLT = {resLT_x, resLT_y};
    pos_t resRB = {resRB_x, resRB_y};
    rect_area1 = RectArea{resLT, resRB};
    return rect_area1;
}


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
    if (right_bottom.first < left_top.first) {
        return {0,0};
    }
    if (right_bottom.second < left_top.second) {
        return {0,0};
    }
    return {right_bottom.first - left_top.first  + 1, right_bottom.second -
                                                      left_top.second + 1};

    //    return dim_t{
//        std::max((size_t) 0, right_bottom.first - left_top.first),
//                 std::max((size_t) 0, right_bottom.second - left_top.second)};
}

bool RectArea::empty() const {
    return size() == dim_t{0, 0};
}

void RectArea::embrace(pos_t pos) {

    if(this->empty()) {
        left_top = pos;
        right_bottom = pos;
        return;
    }
    set_left_top(pos_t(std::min(left_top.first, pos.first),
                       std::min(left_top.second, pos.second)));
    set_right_bottom(pos_t(std::max(right_bottom.first, pos.first),
                           std::max(right_bottom.second, pos.second)));
}


// __________Crossword CLASS IMPLEMENTATION__________

Crossword::Crossword(const Word& w, std::initializer_list<Word> wordList) :
        rect_area(w.rect_area()){
    insert_word(w);
    for (auto i = wordList.begin(); i < wordList.end(); i++) {
        insert_word(*i);
    }

}

Crossword::Crossword(const Crossword& crossword)
        :
        words(crossword.words),
        countV(crossword.countV),
        countH(crossword.countH),
        rect_area(crossword.rect_area),
        fullArea(crossword.fullArea)
{
    if (crossword.countH + crossword.countV == 0) {
        rect_area = DEFAULT_EMPTY_RECT_AREA;
    }
}

Crossword::Crossword(Crossword&& crossword)
        :
        words(std::move(crossword.words)),
        countV(crossword.countV),
        countH(crossword.countH),
        rect_area(std::move(crossword.rect_area)),
        fullArea(std::move(crossword.fullArea))
{
    if (crossword.countH + crossword.countV == 0) {
        rect_area = DEFAULT_EMPTY_RECT_AREA;
    }
}

Crossword Crossword::operator+(const Crossword& cr){
    Crossword newCross{*this};
    for (const auto& w : cr.words) {
        newCross.insert_word(w);
    }
    return newCross;
}



Crossword& Crossword::operator+=(const Crossword& cr){
    for (const auto& w : cr.words){
        (*this).insert_word(w);
    }
    return *this;
}

Crossword& Crossword::operator=(const Crossword& cr){
    words = cr.words;
    rect_area = cr.rect_area;
    countV = cr.countV;
    countH = cr.countH;
    fullArea = cr.fullArea;
    return *this;
}

Crossword& Crossword::operator=(Crossword&& cr){
    words = std::move(cr.words);
    rect_area = std::move(cr.rect_area);
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
    if (word.get_orientation() == H) {
        ++col;
    }
    else {
        ++row;
    }
    if (word.get_start_position().first != 0) {
        pos_t before = std::pair(word.get_start_position().first - col, word.get_start_position().second - row);
        if (fullArea.contains(before)) {
            return false;
        }
    }
    pos_t after = std::pair(word.get_end_position().first + col, word.get_end_position().second + row);
    if(fullArea.contains(after)) {
        return false;
    }

    return true;
}


bool Crossword::collision(const Word& word) {
    RectArea frame = word.get_rect_area_with_frame();

    for (const Word& w : words) {
        RectArea product = w.rect_area() * frame;
        if (!product.empty()) {
            if (word.get_orientation() == w.get_orientation()) {
                if (!product.empty()) {
                    return true;
                }
            }
            else {
                RectArea wordProduct = w.rect_area() * word.rect_area();
                if (wordProduct.empty()) {
                    return true;
                }
                pos_t pos = wordProduct.get_left_top();
                size_t idx = std::max(pos.first - word.get_start_position().first, pos.second - word.get_start_position().second);
                char toCheck = word.getLetter(idx);
                if (toCheck >= 'a' && toCheck <= 'z') {
                    toCheck += ENLARGE;
                } else if (toCheck > 'Z' || toCheck < 'A') {
                    toCheck = DEFAULT_CHAR;
                }
                if (fullArea.contains(pos) && fullArea[pos] != toCheck)
                    return true;
            }
        }
    }
    return false;
}




bool Crossword::insert_word(const Word& word) {
    if (collision(word))
        return false;

    if (word.get_orientation() == orientation_t::H) ++countH;
    else ++countV;

    words.insert(word);

    rect_area.embrace(word.get_start_position());
    rect_area.embrace(word.get_end_position());

    pos_t pos = word.get_start_position();
    size_t idx = 0;
    orientation_t orientation = word.get_orientation();
    while(pos <= word.get_end_position()) {
        char toAdd = word.getLetter(idx);
        if (toAdd >= 'a' && toAdd <= 'z') {
            toAdd += ENLARGE;
        }
        else if (toAdd < 'A' || toAdd > 'Z') {
            toAdd = DEFAULT_CHAR;
        }
        fullArea.insert({pos, toAdd});
        pos = nextPos(pos, orientation);
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

std::ostream& operator <<(std::ostream& out, const Crossword& cross) {
    pos_t pos = cross.rect_area.get_left_top();
    pos_t end = cross.rect_area.get_right_bottom();
    for (auto i = pos.second; i <= end.second; i++) {
        for (auto j = pos.first; j <= end.first; j++){
            if (cross.fullArea.contains({j,i})) {
                char c = cross.fullArea.at({j,i});
                if (c >= 'a' && c <= 'z') {
                    c += ENLARGE;
                }
                else if (c < 'A' || c > 'Z') {
                    c = DEFAULT_CHAR;
                }
                out << c;
                if(j!= end.first) {
                    out << ' ';
                }
            }
            else {
                out << CROSSWORD_BACKGROUND;
                if(j!= end.first) {
                    out << ' ';
                }
            }
        }
        out << '\n';
    }
    return out;
}
