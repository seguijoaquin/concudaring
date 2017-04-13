#ifndef CONCUDARING_DECKOFCARDS_H
#define CONCUDARING_DECKOFCARDS_H
#include <vector>


class DeckOfCards{
private:
    std::vector<int> cards;
public:
    DeckOfCards();
    ~DeckOfCards();
    DeckOfCards& operator+(DeckOfCards anotherDeck);
    DeckOfCards& operator=(DeckOfCards anotherDeck);
    void addCard(int card);
    void print();
    bool isEmpty();
    int at() const;
    bool theCardsAreSame();
    int getCard();
};


#endif //CONCUDARING_DECKOFCARDS_H
