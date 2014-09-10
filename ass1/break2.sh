#!/usr/bin/env bash
for i in {1..26}
do
	./substitution $i < in.txt > out$i.txt
done

