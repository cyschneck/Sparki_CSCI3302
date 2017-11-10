#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov  9 20:11:49 2017

@author: kair
"""
import numpy as np

def main():
    file_10 = open("clean_10cm")
    list_10 = file_10.read()
    list_10 = str.split(list_10)
    results_10 = list(map(int, list_10))
    results_10 = np.array(results_10)
    #print(type(results))
    #print(results)
    variance_10 = np.var(results_10)
    print(variance_10)
    
    file_20 = open("clean_20cm")
    list_20 = file_20.read()
    list_20 = str.split(list_20)
    results_20 = list(map(int, list_20))
    results_20 = np.array(results_20)
    variance_20 = np.var(results_20)
    print(variance_20)
    
    file_30 = open("clean_30cm")
    list_30 = file_30.read()
    list_30 = str.split(list_30)
    results_30 = list(map(int, list_30))
    results_30 = np.array(results_30)
    variance_30 = np.var(results_30)
    print(variance_30)
    
    file_40 = open("clean_40cm")
    list_40 = file_40.read()
    list_40 = str.split(list_40)
    results_40 = list(map(int, list_40))
    results_40 = np.array(results_40)
    variance_40 = np.var(results_40)
    print(variance_40)

if __name__== "__main__":
    main()
    
