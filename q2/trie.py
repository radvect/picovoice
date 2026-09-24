
### https://github.com/radvect/neetcode-submissions/tree/main/Data%20Structures%20%26%20Algorithms/implement-prefix-tree
### Modified for phoenemes
### was needed to replace the word by the list of phonemes and add a stored word at the isend node

class PrefixTree:

    def __init__(self):
        self.root = TrieNode(None)

    def insert(self, phonemes: list, word: str) -> None:
        current = self.root
        for phoneme in phonemes: 
        
            if(phoneme in current.children):
                current = current.children[phoneme]
            else:
                new = TrieNode(phoneme)
                current.children[phoneme] = new
                current = current.children[phoneme]
        current.isEnd=True
        current.word = word   

    def search(self, word: str) -> bool:
        current = self.root
        for symb in word: 
            
            if(symb in current.children):
                current = current.children[symb]
            else:
                return False
        
        return  current.isEnd

    # def startsWith(self, prefix: str) -> bool:
    #     current = self.root
    #     for symb in prefix: 
            
    #         if(symb in current.children):
    #             current = current.children[symb]
    #         else:
    #             return False
    #     return True
        
class TrieNode:
    def __init__(self, phoeneme):
        self.children = dict()
        self.isEnd = False
        self.phoeneme = phoeneme
        self.word = None