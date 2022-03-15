/*******************************************************************/
/*                                                                 */
/*  A1 Part 3: Majority Element starter file                       */
/*                                                                 */
/*  Author1 Name: Junhee Kim                                       */
/*      - All                                                      */
/*                                                                 */
/*******************************************************************/
bool findCandidate(int array[],int& candidate, int sz);
bool majorityElement(const int arr[], int& majority, int sz);

bool majorityElement(const int arr[], int& majority, int sz){
    int temp[sz];
    int count = 0;
    bool found = false;
	
	// Temporary array
    for(int i = 0; i < sz; i++){
        temp[i] = arr[i];
    }
	
	// If the majority element is the candidate.
    if(::findCandidate(temp, majority, sz)){
        for(int i = 0; i < sz; ++i){
			// Count the number of element within the array
            if(arr[i] == majority){
                ++count;
            }
            
			// If count is more than 50%, that is the majority element
            if(count > sz/2){
                found = true;
				i = sz;
            }
        }
    }

    return found;

}
bool findCandidate(int array[],int& candidate, int sz){
	// If there are odd number of array, update the candidate
	if(sz % 2 == 1)
	{
		candidate = array[sz - 1];
	}
	if(sz <= 1)
	{	
		// If there are only 1 remaining in the array, that is the candidate
		if(sz == 1)
		{
			candidate = array[sz - 1];
		}
		return candidate >= 0;
	} 
	
	int index = 0;
	int temp[sz];
	// If the pairs match, add one of them into new array
	for (int i = 0; i < sz - 1; i += 2) {
		if (array[i] == array[i + 1]) {
			temp[index] = array[i];
			index++;
		}
	}
	return findCandidate(temp, candidate, index);
}
