#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

// �J�[�h�̃X�[�g��\���񋓌^
enum class Suit {
    Hearts,
    Diamonds,
    Spades,
    Clubs
};

// �J�[�h�̃����N��\���񋓌^
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

// �J�[�h�N���X
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

// �f�b�L�N���X
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

// �v���C���[�N���X
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
        cout << "���v: " << getHandValue() << endl;
    }

    double calculatePayout(double payoutMultiplier) const {
        return bet * payoutMultiplier;
    }

    const vector<Card>& getHand() const {
        return hand;
    }
};

// �v���C���[���o�[�X�g���Ă��炸�A�f�B�[���[��2�����������_�ł̍��v��17�ȏ�19�ȉ��ŁA�v���C���[���I����������菬�����ꍇ�ɁA�f�B�[���[�������֐�
void dealerDrawCard(Deck& deck, Player& dealer, const Player& player) {
    while (dealer.getHandValue() >= 17 && dealer.getHandValue() <= 19 && dealer.getHandValue() < player.getHandValue()) {

        // �V����1������
        dealer.addCard(deck.drawCard());

        cout << "�f�B�[���[���J�[�h�������܂����F" << endl;
        dealer.displayHand();
    }
}

int main() {
    Deck deck;
    deck.shuffle();

    Player player;
    Player dealer;

    double betAmount;
    cout << "�悤�����I����̓u���b�N�W���b�N�ł��B�|��������͂��Ă�������: ";
    cin >> betAmount;

    player.setBet(betAmount);

    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());
    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());

    cout << "�v���C���[�̎�D:" << endl;
    player.displayHand();
    cout << endl;

    while (true) {
        cout << "�q�b�g���X�^���h���I��ł��������B: (1) �q�b�g(2) �X�^���h" << endl;
        int choice;
        cin >> choice;

        if (choice == 1) {
            player.addCard(deck.drawCard());
            player.displayHand();

            if (player.getHandValue() > 21) {
                cout << "�o�[�X�g���܂����B���Ȃ��̕����ł��B�����߂��͂���܂���B" << endl;
                break;
            }
        }
        else if (choice == 2) {
            cout << "�f�B�[���[�̎�D�F" << endl;
            dealer.displayHand();

            dealerDrawCard(deck, dealer, player);
            while (dealer.getHandValue() < 17) {
                dealer.addCard(deck.drawCard());
                dealer.displayHand();


                while (dealer.getHandValue() >= 17 && dealer.getHandValue() <= 19 && dealer.getHandValue() < player.getHandValue()) {
                    // �f�B�[���[���ǉ��ň��������𖞂����Ă���ꍇ
                    dealer.addCard(deck.drawCard());
                    cout << "�f�B�[���[���J�[�h�������܂����F" << endl;
                    dealer.displayHand();
                }

                if (dealer.getHandValue() > 21) {
                    cout << "�f�B�[���[���o�[�X�g���܂����B���Ȃ��̏����ł�!" << endl;
                    break;
                }
            }

            if (dealer.getHandValue() <= 21 && dealer.getHandValue() >= player.getHandValue()) {
                cout << "���Ȃ��̕����ł��B�����߂��͂���܂���B" << endl;

            }
            else if (player.getHandValue() == 21 && player.getHand().size() == 2) {
                double payout = player.calculatePayout(2.5);
                cout << "�u���b�N�W���b�N!���Ȃ��̏����ł��I�z��1.5�{�ƂȂ�܂��B�����߂���" << payout << "�I" << endl;
            }
            else {
                double payout = player.calculatePayout(2.0);
                cout << "���Ȃ��̏����ł��B�z��1�{�ł��B�����߂��� " << payout << "�I" << endl;
            }
            break;
        }
    }

    return 0;
}
