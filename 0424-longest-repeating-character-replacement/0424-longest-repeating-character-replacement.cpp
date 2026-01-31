class Solution {
public:
    int characterReplacement(string s, int k) {
        unordered_map<char, int> mp;
        
        int left = 0;
        int windowsize =0;
        int maxfreq = 0;
        for(int r=0; r<s.size(); r++){
            mp[s[r]]++;
            maxfreq = max(maxfreq, mp[s[r]]);


            while((r-left+1) - maxfreq > k){ //windowsize - maxfreq
                mp[s[left]]--;
                left++;
                
            }

            windowsize = max(windowsize, r-left+1);
        }
        return windowsize;
        
    }
};