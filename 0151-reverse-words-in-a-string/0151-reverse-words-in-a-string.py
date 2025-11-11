class Solution:
    def reverseWords(self, s: str) -> str:
        temp = s[::-1]
        word, sentence = '', ''
        for char in temp:
            if char != ' ':
                word += char
            elif word:
                word = word[::-1]
                sentence += " "+word
                word = ''
        if word:
            sentence += " "+word[::-1]

        return sentence.strip()
