#include <iostream>
#include "deckOfCards.h"



DeckOfCards::DeckOfCards() {}

DeckOfCards::~DeckOfCards() {}




void DeckOfCards::addCard(int card){
    cards.push_back(card);
}

void DeckOfCards::addDeck(DeckOfCards anotherDeck) {
    int card = anotherDeck.getCard();
    while (!anotherDeck.isEmpty()){
        addCard(card);
        card = anotherDeck.getCard();
    }
}


DeckOfCards& DeckOfCards::operator=(DeckOfCards anotherDeck) {
    std::swap(cards,anotherDeck.cards);
    return *this;
}

void DeckOfCards::print(){
    for (int i = 0; i < cards.size(); ++i) {
        std::cout << cards[i] << ".";
    }
    std::cout << "\n";
}

bool DeckOfCards::isEmpty() {
    return cards.empty();
}

bool DeckOfCards::theCardsAreSame() {
    if (cards.size() == 1){
        return false;
    }else{
        return (cards[0] == cards[1]);
    }
}

//Obtengo la ultima carta pero la saco del maso
int DeckOfCards::at() const {
    int card = cards.back();
    return card;
}

//Saca del maso la ultima carta y la devuelvo
int DeckOfCards::getCard() {
    int card =  cards.back();
    cards.pop_back();
    return card;
}
