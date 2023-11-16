#ifndef crosswords_h
#define crosswords_h

#include <iostream>
#include <set>
#include <map>

extern constinit char DEFAULT_CHAR;
extern constinit std::string_view DEFAULT_WORD;
extern constinit char CROSSWORD_BACKGROUND;

using pos_t = std::pair<size_t, size_t>;
using dim_t = std::pair<size_t, size_t>;
using count_t = std::pair<size_t, size_t>;
enum orientation_t : bool {H, V};

class RectArea {
private:
    pos_t left_top;
    pos_t right_bottom;

public:
    RectArea(const pos_t& lt, const pos_t& rb);
    RectArea(const RectArea &rect_area);
    RectArea(RectArea &&rect_area);
    RectArea(const pos_t& rb) = delete;
    RectArea() = delete;
    constexpr RectArea(size_t lt_x, size_t lt_y, size_t rb_x, size_t rb_y):
            left_top(lt_x, lt_y), right_bottom(rb_x, rb_y) {
    }

    RectArea& operator=(const RectArea& rect_area);
    RectArea& operator=(RectArea&& rect_area);
    friend RectArea operator*(const RectArea& rect_area1,
                              const RectArea& rect_area2);
    friend RectArea& operator*=(RectArea& rect_area1,
                                const RectArea& rect_area2);

    pos_t get_left_top() const;
    void set_left_top(const pos_t& new_pos);
    pos_t get_right_bottom() const;
    void set_right_bottom(const pos_t& new_pos);

    dim_t size() const;
    bool empty() const;
    void embrace(pos_t pos);
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

    Word& operator=(const Word& word);
    Word& operator=(Word&& word);
    bool operator==(const Word& word) const;
    bool operator!=(const Word& word) const;
    std::strong_ordering operator<=>(const Word& word) const;

    pos_t get_start_position() const;
    pos_t get_end_position() const;
    orientation_t get_orientation () const;

    std::string get_str() const;
    size_t length() const;
    char at(size_t ind) const;

    RectArea rect_area() const;
    RectArea get_rect_area_with_frame() const;
};

class Crossword {
private:
    std::set<Word> words;
    size_t countV = 0;
    size_t countH = 0;
    RectArea rect_area;
    std::map<pos_t, char> fullArea;

    static pos_t nextPos(const pos_t& pos, orientation_t orientation);
    static void correctLetter(char& c);
    bool collision(const Word& word);

public:
    Crossword(const Word& w, std::initializer_list<Word> wordList);
    Crossword(const Crossword& crossword);
    Crossword(Crossword&& crossword);

    Crossword& operator=(const Crossword&);
    Crossword& operator=(Crossword&&);
    Crossword operator+(const Crossword&);
    Crossword& operator+=(const Crossword& cr);

    bool insert_word(const Word& word);
    dim_t size() const;
    count_t word_count() const;

    friend std::ostream& operator<<(std::ostream& out, const Crossword& cross);
};

constexpr RectArea DEFAULT_EMPTY_RECT_AREA = RectArea(1,1,0,0);


#endif
