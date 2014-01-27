#!/usr/bin/env python

# Produces a JSON file which contains only the new good runs and lumisections
# given the old JSON file and the new JSON file.
# Usage: python jsondiff.py new.json old.json diff.json
#
# Robin Nandi, 29 May 2010

import sys, json

# Converts [[1,3],[5,9]] to [1,2,3,5,6,7,8,9]
def expand(lumis):
  res=[]
  for lumirange in lumis:
    for lumi in range(lumirange[0],lumirange[1]+1):
      res.append(lumi)
  return res

# Converts [1,2,3,5,6,7,8,9] to [[1,3],[5,9]]
def contract(lumis):
  res=[]
  a=lumis[0]
  b=a-1
  for lumi in lumis:
    if lumi != b+1:
      res.append([a,b])
      a=lumi
    if lumi == lumis[len(lumis)-1]:
      res.append([a,lumi])
    b=lumi
  return res

# Given [[1,4]] and [[1,3],[5,9]] returns [[5,9]]
def diff(a,b):
  setdiff=set(expand(b))-set(expand(a))
  if setdiff == set([]):
    return []
  else:
    return contract(sorted(list(setdiff)))

json_new_name=sys.argv[1]
json_old_name=sys.argv[2]
json_diff_name=sys.argv[3]

jsondiff={}

jsonfile_old=file(json_old_name,'r')
jsondict_old=json.load(jsonfile_old)

jsonfile_new=file(json_new_name,'r')
jsondict_new=json.load(jsonfile_new)

for run in jsondict_new.keys():
  if run not in jsondict_old.keys():
    jsondiff[run]=jsondict_new[run]
  else:
    if jsondict_new[run] != jsondict_old[run]:
      jsondiff[run]=diff(jsondict_old[run],jsondict_new[run])

newfile=open(json_diff_name,'w')
json.dump(jsondiff,newfile)
newfile.close()
