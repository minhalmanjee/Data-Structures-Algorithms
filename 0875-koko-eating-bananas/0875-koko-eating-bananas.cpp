//complexxity: half searches so Log(max of piles) x n (n-> we calculate totalspeed in tf below)
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int s=1;
        int e= *max_element(piles.begin(), piles.end());
        int ans=0;

        while(s<=e){
            int mid = s+(e-s)/2;
            long long time = tf(mid, piles);
            if(time <= h){
                ans = mid;
                e = mid-1;
            }
            else s = mid+1;
        }
        return ans;

        
    }

    long long tf(int mid, vector<int>& piles)
    {
        long long sum=0;
        for(int i=0; i<piles.size(); i++){
            sum+= (piles[i]/mid) + ((piles[i]%mid)!=0);
        }
        return sum;

    }
};






//Bruteforce, speed=O(n+m) where n is number of iterations in this case 11 and m is piles.length because we try every speed starting from 1 to 11 because at 11 because in worst case:
//[11,11,11] h=3 so only speed of 11 bananas/hr will satisfy
// class Solution {
// public:
//     int minEatingSpeed(vector<int>& piles, int h) {
//         int speed=1;
//         int totaltime = h+1;
//         while(totaltime > h){
//             totaltime=0;
//             for(int i=0; i<piles.size(); i++){
//                 totaltime += (piles[i] + speed-1)/speed;
//             }
//             if(totaltime>h){
//                 speed++;
//             }
//         }
//         return speed;
        
//     }
// };