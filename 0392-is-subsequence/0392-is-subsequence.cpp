class Solution {
public:
    bool isSubsequence(string s, string t) {
        if(s == ""){
            return true;
        }
        bool flag = false;
        int j =0;
        for(int i =0; i<t.length(); i++){
            if(s[j] == t[i]){
                j++;
                if(j == s.length()){
                    flag = true;
                    break;
                }
            }
        }
        return flag;
        
    }
};