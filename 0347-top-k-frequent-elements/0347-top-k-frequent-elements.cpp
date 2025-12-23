class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {

        unordered_map<int, int> mp;

        for(int i = 0; i < nums.size(); i++){
            mp[nums[i]]++;
        }

        // considering edge case where nums = [1,1,1,1]
        // unique elements: 1
        // bucket size: 4 because 1 will be placed in 4 freq bucket and 0-3 will be empty so we need
        // n+1 buckets
        int n = nums.size();  // declare n
        vector<vector<int>> bucket(n + 1);  // since vector goes from 0 to n

        for(auto it = mp.begin(); it != mp.end(); it++){
            bucket[it->second].push_back(it->first);
        }

        vector<int> res;

        // Step 3: collect top k frequent numbers
        for(int i = n; i >= 0; i--){
            for(int j = 0; j < bucket[i].size(); j++){
                res.push_back(bucket[i][j]);
                if(res.size() == k){
                    break;
                }
            }

            if(res.size() == k){
                break;
            }
        }

        return res;
    }
};





// class Solution {
// public:
//     vector<int> topKFrequent(vector<int>& nums, int k) {
//         vector<int> values; vector<int> freq;
//         for(int i=0; i<nums.size(); i++){
//             bool found = false;

//             for(int j=0; j<values.size(); j++){
//                 if(values[j] == nums[i]){
//                     freq[j]++;
//                     found = true;
//                     break;
//                 }
//             }

//             if(!found){
//                 values.push_back(nums[i]);
//                 freq.push_back(1);
//             }
//         }


//         vector<int> res;

//         for(int i=0; i<k; i++){
//             int maxindex = 0;

//             for(int j =0; j<freq.size(); j++){
//                 if(freq[j] > freq[maxindex]){
//                     maxindex = j;
//                 }
//             }

//             res.push_back(values[maxindex]);
//             freq[maxindex] = -1;
//         }

//         return res;


        
//     }
// };