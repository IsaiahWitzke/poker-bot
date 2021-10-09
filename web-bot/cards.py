from enum import Enum

class Card:
    def __init__(self, suit, val):
        self.suit = suit
        self.val = val
  
    def to_string(self):
        return self.suit + self.val

    def value(self):
        pass