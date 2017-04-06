#ifndef CONCUDARING_DECKOFCARDS_H
#define CONCUDARING_DECKOFCARDS_H
#include <vector>


class DeckOfCards{
private:
    std::vector<int> cards;
public:
    DeckOfCards();
    ~DeckOfCards();
    void addDeckOfCards(DeckOfCards& otherDeckOfCards);
    void addCard(int card);
    int getCard();
};


#endif //CONCUDARING_DECKOFCARDS_H
