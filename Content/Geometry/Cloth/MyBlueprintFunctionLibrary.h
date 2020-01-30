// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "Algo/Reverse.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "SkeletalRenderPublic.h"
#include "ReferenceSkeleton.h"
#include "Components/SkinnedMeshComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkinWeightVertexBuffer.h"

#include "Modules/ModuleManager.h"
#include "RenderingThread.h"
#include "LoaderBPFunctionLibrary.h"


#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SHOPPERS_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	static USkeletalMeshComponent* _skelStatic;
	static TArray<uint32> oldIndexBuffer2;
	static int oldVerticesNum;

	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static TArray<FVector> AdjustClothV5(USkeletalMeshComponent * _refAvatar, USkeletalMeshComponent * _refCloth, USkeletalMeshComponent * _targetAvatar, USkeletalMeshComponent * _targetCloth, float _amount, int _vertCount, TArray<int> ignoreBones, UPoseableMeshComponent * poseableMesh);
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void ResetVertices(USkeletalMeshComponent *_meshComp, TArray<FVector> _oldVertices);
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static FName GetRelevantBone(UPoseableMeshComponent* _mesh, FName _boneIndex);

	static TArray<TArray<int32>> FindChildrenBones(USkeletalMeshComponent * _meshComp);
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void IncreaseIndicesCapacity1(USkeletalMeshComponent * _refAvatar);
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void IncreaseIndicesCapacity2(USkeletalMeshComponent * _refAvatar);
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		static void LoadSkeletalMesh(USkeletalMeshComponent * _refAvatar, FReturnedData importedData);
	static FName GetRelevantBone(USkeletalMeshComponent * _mesh, FName _bone);
	static int GetNearestBone(USkeletalMeshComponent * _mesh, TArray<int> _fromBones, FVector _pos);

};
