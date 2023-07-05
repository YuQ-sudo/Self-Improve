#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <limits>
using namespace std;
void bubble_sort(std::vector<int>& input)
{
    for(int i = 1; i < input.size();i++)
    {
        for(int j = 0;j < input.size();j++)
        {
            if(input[i] < input[j])
            {
                std::swap(input[i],input[j]);
            }
        }
    }

};
void insert_sort(std::vector<int>& input)
{
    for(int i = 1;i < input.size();i++)
    {
        // int value = input[i];
        int j = i;
        while(j--)
        {
            if(input[j] < input[j-1])
            {
                std::swap(input[j],input[j-1]);
            }
            else
                break;
        }

    }

};

void Merge(vector<int> &Array, int front, int mid, int end) {
    std::vector<int> left;
    std::vector<int> right;
    for(int i = front;i<=mid;i++)
    {
        left.push_back(Array[i]);
    }
    for(int i = mid+1;i<=end;i++)
    {
        right.push_back(Array[i]);
    }
    std::vector<int> t;
    int i = 0;
    int j = 0;
    while(i<left.size() || j<right.size())
    {
        if(i < left.size() && j <right.size())
        {
            if(left[i] < right[j])
            {
                t.push_back(left[i]);
                i++;
            }
            else
            {
                t.push_back(right[j]);
                j++;
            }
        }
        else if( i == left.size())
        {
            t.push_back(right[j]);
            j++;
        }
        else if(j == right.size())
        {
            t.push_back(left[i]);
            i++;
        }
    }
    for(int k = front;k<=end;k++)
    {
        Array[k] = t[k - front];
    }
    std::cout<<std::endl;
};

void MergeSort(vector<int> &Array, int front, int end) {
    if (front >= end)
        return;
    int mid = (front + end) / 2;
    MergeSort(Array, front, mid);
    MergeSort(Array, mid + 1, end);
    Merge(Array, front, mid, end);
};
// 快速排序
// 从数列中挑出一个元素，称为 "基准"（pivot）;
// 重新排序数列，所有元素比基准值小的摆放在基准前面，所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。在这个分区退出之后，该基准就处于数列的中间位置。这个称为分区（partition）操作；
// 递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序；


void QuickSort(vector<int> &Array,int front, int end)
{
    if(front >= end) return;

    int i = front;
    int j = end;
    int tmp = Array[front];
    while(i != j){
        while(Array[j] >= tmp && j > i)
            j--;
        while(Array[i] <= tmp && j > i)
            i++;
        if(j > i){
            std::swap(Array[i],Array[j]);
        }
    }
    Array[front] = Array[i];
    Array[i] = tmp;
    QuickSort(Array, front, i-1);
    QuickSort(Array, i+1, end);
};
int main()
{
    std::vector<int> test_num{ 22, 34, 3, 32, 82, 55, 89, 50, 37, 5, 64, 35, 9, 70};
    // QuickSort(test_num,0,test_num.size()-1);
    // insert_sort(test_num);
    for(auto n : test_num)
    {
        std::cout << n <<std::endl;
    }
    return 0;
}