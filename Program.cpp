#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

// カードのスートを表す列挙型
enum class Suit {
    Hearts,
    Diamonds,
    Spades,
    Clubs
};

// カードのランクを表す列挙型
enum class Rank {
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

// カードクラス
class Card {
private:
    Suit suit;
    Rank rank;

public:
    Card(Suit s, Rank r) : suit(s), rank(r) {}

    int getRankValue() const {
        return static_cast<int>(rank);
    }

    void display() const {
        string suitStr;
        string rankStr;

        switch (suit) {
        case Suit::Hearts:
            suitStr = "Hearts";
            break;
        case Suit::Diamonds:
            suitStr = "Diamonds";
            break;
        case Suit::Spades:
            suitStr = "Spades";
            break;
        case Suit::Clubs:
            suitStr = "Clubs";
            break;
        }

        switch (rank) {
        case Rank::Ace:
            rankStr = "Ace";
            break;
        case Rank::Jack:
            rankStr = "Jack";
            break;
        case Rank::Queen:
            rankStr = "Queen";
            break;
        case Rank::King:
            rankStr = "King";
            break;
        default:
            rankStr = to_string(static_cast<int>(rank));
            break;
        }

        cout << rankStr << " of " << suitStr << endl;
    }
};

// デッキクラス
class Deck {
private:
    vector<Card> cards;
    using Iterator = vector<Card>::iterator;

public:
    Deck() {
        for (int s = 0; s < 4; ++s) {
            for (int r = 1; r <= 13; ++r) {
                Suit suit = static_cast<Suit>(s);
                Rank rank = static_cast<Rank>(r);
                Card card(suit, rank);
                cards.push_back(card);
            }
        }
    }

    void shuffle() {
        random_device rd;
        mt19937 gen(rd());
        std::shuffle(cards.begin(), cards.end(), gen);
    }

    Card drawCard() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    Iterator begin() {
        return cards.begin();
    }

    Iterator end() {
        return cards.end();
    }
};

// プレイヤークラス
class Player {
private:
    vector<Card> hand;
    double bet = 0.0;

public:
    void setBet(double amount) {
        bet = amount;
    }

    double getBet() const {
        return bet;
    }

    void addCard(Card card) {
        hand.push_back(card);
    }

    int getHandValue() const {
        int value = 0;
        int aceCount = 0;

        for (const Card& card : hand) {
            int rankValue = card.getRankValue();

            if (rankValue == 1) {
                ++aceCount;
                value += 11;
            }
            else if (rankValue > 10) {
                value += 10;
            }
            else {
                value += rankValue;
            }
        }

        while (value > 21 && aceCount > 0) {
            value -= 10;
            --aceCount;
        }

        return value;
    }

    void displayHand() const {
        for (const Card& card : hand) {
            card.display();
        }
        cout << "合計: " << getHandValue() << endl;
    }

    double calculatePayout(double payoutMultiplier) const {
        return bet * payoutMultiplier;
    }

    const vector<Card>& getHand() const {
        return hand;
    }
};

// プレイヤーがバーストしておらず、ディーラーが2枚引いた時点での合計が17以上19以下で、プレイヤーが終了した数より小さい場合に、ディーラーが引く関数
void dealerDrawCard(Deck& deck, Player& dealer, const Player& player) {
    while (dealer.getHandValue() >= 17 && dealer.getHandValue() <= 19 && dealer.getHandValue() < player.getHandValue()) {

        // 新しく1枚引く
        dealer.addCard(deck.drawCard());

        cout << "ディーラーがカードを引きました：" << endl;
        dealer.displayHand();
    }
}

int main() {
    Deck deck;
    deck.shuffle();

    Player player;
    Player dealer;

    double betAmount;
    cout << "ようこそ！これはブラックジャックです。掛け金を入力してください: ";
    cin >> betAmount;

    player.setBet(betAmount);

    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());
    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());

    cout << "プレイヤーの手札:" << endl;
    player.displayHand();
    cout << endl;

    while (true) {
        cout << "ヒットかスタンドか選んでください。: (1) ヒット(2) スタンド" << endl;
        int choice;
        cin >> choice;

        if (choice == 1) {
            player.addCard(deck.drawCard());
            player.displayHand();

            if (player.getHandValue() > 21) {
                cout << "バーストしました。あなたの負けです。払い戻しはありません。" << endl;
                break;
            }
        }
        else if (choice == 2) {
            cout << "ディーラーの手札：" << endl;
            dealer.displayHand();

            dealerDrawCard(deck, dealer, player);
            while (dealer.getHandValue() < 17) {
                dealer.addCard(deck.drawCard());
                dealer.displayHand();


                while (dealer.getHandValue() >= 17 && dealer.getHandValue() <= 19 && dealer.getHandValue() < player.getHandValue()) {
                    // ディーラーが追加で引く条件を満たしている場合
                    dealer.addCard(deck.drawCard());
                    cout << "ディーラーがカードを引きました：" << endl;
                    dealer.displayHand();
                }

                if (dealer.getHandValue() > 21) {
                    cout << "ディーラーがバーストしました。あなたの勝ちです!" << endl;
                    break;
                }
            }

            if (dealer.getHandValue() <= 21 && dealer.getHandValue() >= player.getHandValue()) {
                cout << "あなたの負けです。払い戻しはありません。" << endl;

            }
            else if (player.getHandValue() == 21 && player.getHand().size() == 2) {
                double payout = player.calculatePayout(2.5);
                cout << "ブラックジャック!あなたの勝ちです！配当1.5倍となります。払い戻しは" << payout << "！" << endl;
            }
            else {
                double payout = player.calculatePayout(2.0);
                cout << "あなたの勝ちです。配当1倍です。払い戻しは " << payout << "！" << endl;
            }
            break;
        }
    }

    return 0;
}
