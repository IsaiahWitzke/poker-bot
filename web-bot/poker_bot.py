from selenium.webdriver.common.keys import Keys

class PokerBot:
    is_folded = True
    
    def __init__(self, name, balance):
        self.name = name
        self.balance = balance
        self.is_folded = True
        self.hand = []

    def new_hand(self, card_1, card_2):
        self.hand.append(card_1)
        self.hand.append(card_2)
        self.is_folded = False

    def add_card_to_hand(self, new_card):
        self.hand.append(new_card)

    def fold(self):
        self.is_folded = True
        self.hand = []
        return ["f", "f"]
    
    def check(self):
        return ["c"]

    def call(self, call_amount):
        self.balance = max([0.0, self.balance - call_amount])
        return ["c"]

    def bet(self, bet_amount):
        validated_bet_amount = bet_amount
        if self.balance < bet_amount:
            validated_bet_amount = self.balance
        self.balance -= validated_bet_amount
        return ["r", "e", str(validated_bet_amount), "r"]

    def eval_hand(self):
        pass

    def play_turn(self):
        """
        Will return an array of keystrokes/letters/numbers that represent what the bot wants to do
        """
        return self.bet(0.2)


