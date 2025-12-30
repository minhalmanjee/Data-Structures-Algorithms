class Solution {
public:
    int search(vector<int>& a, int t) {
        int s = 0; int e=a.size()-1;

        while(s <= e){
            int m = s + (e-s)/2;
            if(a[m] == t){
                return m;
            }


            if(a[s] <= a[m]){ //left sorted
                if(a[s] <= t && t <= a[m]){
                    e = m-1;
                }
                else{
                    s = m+1;
                }
            }
            else{
                if(a[m] <= t && t <= a[e]){
                    s=m+1;
                }
                else{
                    e=m-1;
                }
            }
        }

        return -1;

        
    }
};