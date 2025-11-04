class Solution {
public:
    bool carPooling(vector<vector<int>>& trips, int capacity) {
        map<int, int>m;
        for (int i=0; i<trips.size(); i++){
            int num = trips[i][0];
            int start = trips[i][1];
            int end = trips[i][2];
            m[start] += num;
            m[end] -= num;
        }

        int curr = 0;
        for(map<int, int>::iterator it=m.begin(); it != m.end(); ++it){
            curr += it->second;
            if(curr > capacity){
                return false;
            }
        }
        return true;
        
    }
};