class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        //going backward for both arrs storing larger element in the arr.

        m=m-1;
        n=n-1;
        int len=nums1.size()-1;

        while(len >= 0 && m>=0 && n>=0){

            if(nums1[m] > nums2[n]){
                nums1[len] = nums1[m];
                m--;
                len--;
            }
            else{
                nums1[len] = nums2[n];
                n--; len--;
            }
            

        }


        while(len >= 0 && m<0 && n>=0){
                nums1[len] = nums2[n];
                n--;
                len--;
            }




        


    }


    
};