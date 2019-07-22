#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <algorithm>    // std::sort
#include <iostream>     // std::cout

using namespace std;

//struct used to store information of suffix
struct suffix
{
	int index; //store original index
	int rank[2]; //store ranks and next pair 
};

//compare two suffxies ,compare 2 pairs,return 1 if first pair is smaller than the second one 
int cmp(struct suffix a,struct suffix b)
{
	return (a.rank[0] == b.rank[0])? (a.rank[1] < b.rank[1] ?1:0):(a.rank[0] < b.rank[0] ?1: 0);
}

//this is the main function takes a string 'txt' of size n and build the suffix array for given string
vector<int> buildSuffixArray(string txt,int n)
{
	//store suffixes and their indexes
	struct suffix suffixes[n]; 

	//store suffix and their indexes in array of certain structure
	//the structure is neede to sort suffixes alphabatically
	//and maintain their old indexe while sorting
	for(int i =0;i<n;i++)
	{
		suffixes[i].index = i;
		suffixes[i].rank[0] = txt[i] - 'a';
		suffixes[i].rank[1] = ((i+1) < n)? (txt[i + 1] - 'a'): -1; 
	}
	//sort them 
	sort(suffixes, suffixes+n, cmp);

	 // At his point, all suffixes are sorted according to first
    	// 2 characters.  Let us sort suffixes according to first 4
	// characters, then first 8 and so on 
	
	int ind[n]; //get index in sufix[] from original index.
    int k = 4;
	for (k; k < 2*n; k = k*2) 
    { 
        	// Assigning rank and index values to first suffix 
        	int rank = 0; 
        	int prev_rank = suffixes[0].rank[0]; 
        	suffixes[0].rank[0] = rank; 
        	ind[suffixes[0].index] = 0;
            // Assigning rank to suffixes 
            for (int i = 1; i < n; i++) 
            { 
                // If first rank and next ranks are same as that of previous 
                // suffix in array, assign the same new rank to this suffix 
                if (suffixes[i].rank[0] == prev_rank && 
                        suffixes[i].rank[1] == suffixes[i-1].rank[1]) 
                { 
                    prev_rank = suffixes[i].rank[0]; 
                    suffixes[i].rank[0] = rank; 
                } 
                else // Otherwise increment rank and assign 
                { 
                    prev_rank = suffixes[i].rank[0]; 
                    suffixes[i].rank[0] = ++rank; 
                } 
                ind[suffixes[i].index] = i; 
            } 
    
            // Assign next rank to every suffix 
            for (int i = 0; i < n; i++) 
            { 
                int nextindex = suffixes[i].index + k/2; 
                suffixes[i].rank[1] = (nextindex < n)? 
                                    suffixes[ind[nextindex]].rank[0]: -1; 
            } 
    
            // Sort the suffixes according to first k characters 
            sort(suffixes, suffixes+n, cmp); 
        } 
    
        // Store indexes of all sorted suffixes in the suffix array 
        vector<int>suffixArr; 
        for (int i = 0; i < n; i++) 
            suffixArr.push_back(suffixes[i].index); 
    
        // Return the suffix array 
        return  suffixArr; 
}

/* To construct and return LCP */
vector<int> kasai(string txt, vector<int> suffixArr) 
{ 
    int n = suffixArr.size(); 
  
    // To store LCP array 
    vector<int> lcp(n, 0); 
  
    // An auxiliary array to store inverse of suffix array 
    // elements. For example if suffixArr[0] is 5, the 
    // invSuff[5] would store 0.  This is used to get next 
    // suffix string from suffix array. 
    vector<int> invSuff(n, 0); 
  
    // Fill values in invSuff[] 
    for (int i=0; i < n; i++) 
        invSuff[suffixArr[i]] = i; 
  
    // Initialize length of previous LCP 
    int k = 0; 
  
    // Process all suffixes one by one starting from 
    // first suffix in txt[] 
    for (int i=0; i<n; i++) 
    { 
        /* If the current suffix is at n-1, then we don’t 
           have next substring to consider. So lcp is not 
           defined for this substring, we put zero. */
        if (invSuff[i] == n-1) 
        { 
            k = 0; 
            continue; 
        } 
  
        /* j contains index of the next substring to 
           be considered  to compare with the present 
           substring, i.e., next string in suffix array */
        int j = suffixArr[invSuff[i]+1]; 
  
        // Directly start matching from k'th index as 
        // at-least k-1 characters will match 
        while (i+k<n && j+k<n && txt[i+k]==txt[j+k]) 
            k++; 
  
        lcp[invSuff[i]] = k; // lcp for the present suffix. 
  
        // Deleting the starting character from the string. 
        if (k>0) 
            k--; 
    } 
  
    // return the constructed lcp array 
    return lcp; 
} 

void printArr(vector<int>arr,int n)
{
    for(int i =0;i<n;i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Driver program 
int main() 
{ 
    string str = "banana"; 
  
    vector<int>suffixArr = buildSuffixArray(str, str.length()); 
    int n = suffixArr.size(); 
  
    cout << "Suffix Array : \n"; 
    printArr(suffixArr, n); 
  
    vector<int>lcp = kasai(str, suffixArr); 
  
    cout << "\nLCP Array : \n"; 
    printArr(lcp, n); 
    return 0; 
} 