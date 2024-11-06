#!/bin/bash


# #for 32Ar
Runs=("057" "058" "059" "061" "062" "064" "065" "066" "067" "068" "069" "070" "071" "072" "074" "077" "112" "113" "114" "115" "116" "118")

for run in "${Runs[@]}"
do
    Fast2Root ../../../../../run/media/local1/Disque_Dur/2024_DATA/DETECTOR_DATA/run_${run}_multifast_32Ar.fast/run_${run}_multifast_32Ar_0001.fast -o ../../../../../../mnt/hgfs/shared-2/2024_DATA/DETECTOR_DATA/ROOT/
done

#for 32Ar thick
# Runs=("075" "076")

# for run in "${Runs[@]}"
# do
#     Fast2Root ../../../../../run/media/local1/Disque_Dur/2024_DATA/DETECTOR_DATA/run_${run}_multifast_32Ar.fast/run_${run}_multifast_32Ar_0001.fast -o ../../../../../../mnt/hgfs/shared-2/2024_DATA/DETECTOR_DATA/ROOT/
# done


# #for 33Ar
# Runs=("078")

# for run in "${Runs[@]}"
# do
#     Fast2Root ../../../../../run/media/local1/Disque_Dur/2024_DATA/DETECTOR_DATA/run_${run}_multifast_33Ar.fast/run_${run}_multifast_33Ar_0001.fast -o ../../../../../../mnt/hgfs/shared-2/2024_DATA/DETECTOR_DATA/ROOT/
# done



#darkcounts without beam
# Runs=("092" "093" "094" "095" "096" "097" "098" "099" "100" "101" "102" "103" "104" "105" "106" "107" "108" "109")
# for run in "${Runs[@]}"
# do
#     Fast2Root ../../../../../run/media/local1/Disque_Dur/2024_DATA/DETECTOR_DATA/run_${run}_lossless_NoBeam.fast/run_${run}_lossless_NoBeam_0001.fast -o ../../../../../run/media/local1/Disque_Dur/2024_DATA/DETECTOR_DATA/run_${run}_lossless_NoBeam.fast/
# done