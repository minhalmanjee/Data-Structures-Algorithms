class Solution {
public:
    int largestRectangleArea(vector<int>& h) {
        int n = h.size();
        stack<int> s;
        vector<int> r(n, 0);
        vector<int> l(n,0);
        

        //right smaller
        for(int i=n-1; i>=0; i--){
            while(s.size() > 0 && h[s.top()] >= h[i]){
                s.pop();
            }
            if(s.size() == 0){
                r[i] = n;
            }
            else{
                r[i] = s.top();
            }
            s.push(i);
        }

        while(!s.empty()){
            s.pop();
        }

        //left smaller
        for(int i=0; i<n; i++){
            while(s.size() > 0 && h[s.top()] >= h[i]){
                s.pop();
            }
            if(s.size() == 0){
                l[i] = -1;
            }
            else{
                l[i] = s.top();
            }
            s.push(i);
        }

        int ans=0;
        for(int i=0; i<n; i++){
            int width = r[i] - l[i] - 1;
            int curr = h[i] * width;
            ans = max(ans, curr);
        }
        return ans;



        
    }
};