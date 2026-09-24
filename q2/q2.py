from typing import Sequence
import trie


trie = trie.PrefixTree() #initilizing tree(structure was taken from a neetcode problem)

trie.insert(["AE", "B", "AH", "K", "AH", "S"], "ABACUS")
trie.insert(["B", "UH", "K"], "BOOK")

trie.insert(["DH", "EH", "R"], "THEIR")
trie.insert(["DH", "EH", "R"], "THERE")

trie.insert(["T", "AH", "M", "AA", "T", "OW"], "TOMATO")
trie.insert(["T", "AH", "M", "EY", "T", "OW"], "TOMATO")


def find_word_combos_with_pronunciation(phonemes: Sequence[str]) -> Sequence[Sequence[str]]:
    res = []
    path = []

    def backtracking(start):

        if (start == len(phonemes)): ###once we went through the all options of phonemes and the words are valid
            res.append(path.copy())
            return

        current = trie.root
        for i in range(start ,len(phonemes)): 

            if(phonemes[i] not in current.children):
                break 
            current = current.children[phonemes[i]]
            if(current.isEnd==True): #check whether we reached the final point of the word

                for word in current.words: #going over different phonemic interpretation
                    path.append(word)
                    backtracking(i + 1) # starting searching for a new word
                    path.pop()

            

    backtracking(0)

    return res 


print(find_word_combos_with_pronunciation(["DH", "EH", "R", "DH", "EH", "R"]))
print(find_word_combos_with_pronunciation(["DH", "EH", "R", "B", "UH", "K"]))
print(find_word_combos_with_pronunciation([ "B", "UH", "K","DH", "EH", "R",]))
print(find_word_combos_with_pronunciation(["T", "AH", "M", "EY", "T", "OW","B", "UH", "K"] ))
print(find_word_combos_with_pronunciation(["T", "AH", "M", "EY", "T", "OW","B", "UH", "K", "T", "AH", "M", "AA", "T", "OW"] ))
print(find_word_combos_with_pronunciation(["DH", "EH", "R", "DH", "EH", "R", "DH", "EH", "R"]))