class Solution:
    def twoSum(self, numbers: List[int], target: int) -> List[int]:
        nlist = []
        n = len(numbers)
        r = n-1
        l = 0
        while l<r:
            tempsum = 0
            tempsum = numbers[l] + numbers[r]
            if tempsum > target:
                r -=1 
            elif tempsum < target:
                l +=1 
            elif tempsum == target:
                nlist.append(l+1)
                nlist.append(r+1)
                return nlist
           