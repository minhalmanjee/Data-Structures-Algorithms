class Solution {
public:
    bool isValid(string s) {
        stack<char> st;

        for(int i=0; i<s.size(); i++){
            //opening case
            if(s[i] == '[' || s[i] == '{' || s[i] == '('){
                st.push(s[i]);
            }
            //closing
            else {
                if(st.size() == 0){
                    return false; //opening but no closing
                }
                if(st.top() == '(' && s[i] == ')'
                || st.top() == '[' && s[i] == ']'
                || st.top() == '{' && s[i] == '}'){
                st.pop();
            } else return false;


            }
            
        }

        return st.size() == 0;
        
    }
};