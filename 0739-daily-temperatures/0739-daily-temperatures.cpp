class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& t) {
        stack<int> s;
        int n = t.size();
        vector<int> d(n, 0);
        for(int i=n-1; i>=0; i--){
            while(s.size() > 0 && t[s.top()] <= t[i]){
                s.pop();
            }
            if(s.size() == 0){
                d[i] = 0;
            }
            else{
                d[i] = s.top() - i;
            }
            s.push(i);
        }
        return d;
        




        
        
    }
};


//BRUTERFORCE

// class Solution {
// public:
//     vector<int> dailyTemperatures(vector<int>& t) {

//         int n = t.size();
//         vector<int> d(n, 0);


//         for(int i=0; i<n; i++){
//             for(int j=i+1; j<n; j++){
//                 if(t[j] > t[i]){
//                     d[i] = j-i;
//                     break;
//                 }
//             }
//         }
//         return d;
        
//     }
// };