class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> mp;

        for(int i=0; i<strs.size(); i++){
            string s = strs[i];
            quicksort(s, 0, s.size()-1);
            mp[s].push_back(strs[i]);
        }

        vector<vector<string>> res;
        for(auto it = mp.begin(); it!=mp.end(); it++){
            res.push_back(it->second);
        }
        return res;



        
    }


    void quicksort(string& s, int l, int h){
        if(l<h){
            int p = partition(s, l, h);
            quicksort(s, l, p-1);
            quicksort(s, p+1, h);
        }
    }

    int partition(string& s, int l, int h){
        int i = l; int j = h;
        int pivot = s[i];
        while(i<j){
            while(i<=j && s[i] <= pivot){
                i++;
            }
            while(s[j] > pivot){
                j--;
            }
            if(i<j){
                swap(s[i], s[j]);
            }
        }
        swap(s[l], s[j]);
        return j;
    }
};


//Bruteforce
// class Solution {
// public:
//     vector<vector<string>> groupAnagrams(vector<string>& strs) {
//         vector<vector<string>> res;
//         vector<string> keys;

//         for(int i = 0; i < strs.size(); i++){
//             string s = strs[i];
//             quicksort(s, 0, s.size() - 1);

//             bool found = false;

//             for(int j = 0; j < keys.size(); j++){
//                 if(keys[j] == s){
//                     res[j].push_back(strs[i]);
//                     found = true;
//                     break;
//                 }
//             }

//             if(found == false){
//                 keys.push_back(s);
//                 res.push_back(vector<string>{strs[i]}); // fixed
//             }
//         }

//         return res;
//     }

//     void quicksort(string &s, int l, int h){
//         if(l < h){
//             int p = partition(s, l, h);
//             quicksort(s, l, p - 1);
//             quicksort(s, p + 1, h);
//         }
//     }

//     int partition(string &s, int l, int h){
//         char pivot = s[h];
//         int i = l - 1;

//         for(int j = l; j < h; j++){
//             if(s[j] <= pivot){
//                 i++;
//                 swap(s[i], s[j]);
//             }
//         }

//         swap(s[i + 1], s[h]);
//         return i + 1;
//     }
// };
