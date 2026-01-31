class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        set<char> sc;
        int maxlen = 0;
        int left = 0;
        for(int r=0; r<s.size(); r++){
            
            while(sc.count(s[r])){
                sc.erase(s[left]);
                left++;
            }
            sc.insert(s[r]);
            maxlen = max(maxlen, r-left+1);
        }
        return maxlen;
        
    }
};