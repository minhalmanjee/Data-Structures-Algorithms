class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.length() != t.length())
            return false;

        quicksort(s, 0, s.length() - 1);
        quicksort(t, 0, t.length() - 1);

        for (int i = 0; i < s.length(); i++) {
            if (s[i] != t[i])
                return false;
        }
        return true;
    }
int partition(string &s, int l, int r) {
    char pivot = s[l];
    int i = l + 1;
    int j = r;

    while (i <= j) {
        while (i <= r && s[i] <= pivot)
            i++;

        while (s[j] > pivot)
            j--;

        if (i < j)
            swap(s[i], s[j]);
    }

    swap(s[l], s[j]);
    return j;
}

void quicksort(string &s, int l, int r) {
    if (l < r) {
        int p = partition(s, l, r);
        quicksort(s, l, p - 1);
        quicksort(s, p + 1, r);
    }
}

};
