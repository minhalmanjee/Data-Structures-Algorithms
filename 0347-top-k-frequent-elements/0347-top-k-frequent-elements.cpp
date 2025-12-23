class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        vector<int> values; vector<int> freq;
        for(int i=0; i<nums.size(); i++){
            bool found = false;

            for(int j=0; j<values.size(); j++){
                if(values[j] == nums[i]){
                    freq[j]++;
                    found = true;
                    break;
                }
            }

            if(!found){
                values.push_back(nums[i]);
                freq.push_back(1);
            }
        }


        vector<int> res;

        for(int i=0; i<k; i++){
            int maxindex = 0;

            for(int j =0; j<freq.size(); j++){
                if(freq[j] > freq[maxindex]){
                    maxindex = j;
                }
            }

            res.push_back(values[maxindex]);
            freq[maxindex] = -1;
        }

        return res;


        
    }
};