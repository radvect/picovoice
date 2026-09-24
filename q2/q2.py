from typing import Sequence
import trie


trie = trie.PrefixTree()

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
        if (start == len(phonemes)):
            res.append(path.copy())
            return


        for i in range(start ,len(phonemes)):
             = phonemes[start:i+1]
            
            
                path.append(phonemes[i])
            

            backtracking(i+1)
            path.pop()


    backtracking(0)

    return res 