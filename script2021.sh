#!/bin/bash

for r in $(seq -f "%03g" 1 56); do
	Fast2Root ../../../../../run/media/local1/T7/Samuel/Regrouped/run_${r}_32Ar_0001.fast -o ../../../../../run/media/local1/T7/Samuel/Regrouped/ROOT/
done
