// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
USkeletalMeshComponent* UMyBlueprintFunctionLibrary::_skelStatic;
TArray<uint32>  UMyBlueprintFunctionLibrary::oldIndexBuffer2;
int UMyBlueprintFunctionLibrary::oldVerticesNum;

TArray<FVector> UMyBlueprintFunctionLibrary::AdjustClothV5(USkeletalMeshComponent * _refAvatar, USkeletalMeshComponent * _refCloth,
	USkeletalMeshComponent * _targetAvatar, USkeletalMeshComponent * _targetCloth,
	float _amount, int _vertCount, TArray<int> ignoreBones, UPoseableMeshComponent * poseableMesh)
{
	UE_LOG(LogTemp, Warning, TEXT("@ AdjustClothV5"));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50, FColor::Red, TEXT("@ AdjustClothV5"));

	TArray<FVector> _oldVerts = TArray<FVector>();

	USkeletalMesh *_mesh = _targetCloth->SkeletalMesh;
	TArray<FFinalSkinVertex> targetClothSkinnedVertices;
	_targetCloth->GetCPUSkinnedVertices(targetClothSkinnedVertices, 0);
	TArray<TArray<int32>> childBones = FindChildrenBones(_refCloth);
	/*for (int sec = 0; sec < _mesh->GetImportedModel()->LODModels[0].Sections.Num(); sec++)
	{*/

	//TArray<FSoftSkinVertex> &_vertices = _mesh->GetImportedModel()->LODModels[0].Sections[sec].SoftVertices;
	//TArray<FVector> &_vertices = _mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50, FColor::Red, TEXT("1 ")
		+ FString::FromInt(_mesh->GetResourceForRendering()->LODRenderData.Num())
		+ FString::FromInt(_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.GetNumVertices()));
	USkeletalMesh *_targetAvatarMesh = _targetAvatar->SkeletalMesh;
	TArray<FColor> _targetMeshColors = TArray<FColor>();
	/*_targetAvatarMesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.ColorVertexBuffer.GetVertexColors(_targetMeshColors);
	_targetAvatarMesh->GetResourceForRendering()->LODRenderData[0].ind;*/
	for (int i = 0; i < (int)_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.GetNumVertices(); i++)
	{
		//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), _vertices[i].Position, 12, FColor::Black, true, 1, 0);
		FVector &vert = _mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i);
		_oldVerts.Add(vert);
		TSkinWeightInfo<false> *weightInfo = _mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer.GetSkinWeightPtr<false>(i);

		int maxBoneIndex = static_cast<int> (weightInfo->InfluenceBones[0]);
		int maxWeight = static_cast<int> (weightInfo->InfluenceWeights[0]);
		for (int j = 0; j < weightInfo->NumInfluences; j++)
		{
			int bIndex = static_cast<int> (weightInfo->InfluenceBones[j]);
			int boneWeight = static_cast<int> (weightInfo->InfluenceWeights[j]);
			if (boneWeight > maxWeight) {
				maxBoneIndex = bIndex;
				maxWeight = boneWeight;
			}
		}
		int secInd, outI;
		_mesh->GetResourceForRendering()->LODRenderData[0].GetSectionFromVertexIndex(i, secInd, outI);
		maxBoneIndex = _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[secInd].BoneMap[maxBoneIndex];
		//maxBoneIndex = _mesh->GetImportedModel()->LODModels[0].Sections[sec].BoneMap[maxBoneIndex];
		FName maxBoneName = _targetCloth->GetBoneName(maxBoneIndex);
		FVector maxBonePosition = _refCloth->GetOwner()->GetTransform().TransformPosition(_refCloth->GetBoneLocation(maxBoneName, EBoneSpaces::ComponentSpace));
		//UE_LOG(LogTemp, Warning, TEXT("@ maxBoneName ## %s %s"), *maxBoneName.ToString(), *_refCloth->GetBoneLocation(maxBoneName, EBoneSpaces::ComponentSpace).ToString());
		FVector maxBonePosition2 = _targetAvatar->GetOwner()->GetTransform().TransformPosition(_targetAvatar->GetBoneLocation(maxBoneName, EBoneSpaces::ComponentSpace));
		//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), maxBonePosition, 10, FColor::Red, true, 1, 0);
		//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), maxBonePosition2, 10, FColor::Cyan, true, 1, 0);
		bool cond = false;
		/*cond = maxBoneName.Compare(FName("upperarm_l")) == 0;
		cond = cond || maxBoneName.Compare(FName("lowerarm_l")) == 0;*/
		cond = cond || maxBoneName.Compare(FName("upperarm_r")) == 0;
		cond = cond || maxBoneName.Compare(FName("lowerarm_r")) == 0;
		cond = cond || maxBoneName.Compare(FName("hand_r")) == 0;
		FColor _col = FColor::Black;
		if (true) {
			//TArray<int32> childrenBones = TArray<int32>();
			TArray<int32> childrenBones = childBones[maxBoneIndex];

			//_refCloth->Bone
			//int tInt = _mesh->Skeleton->GetChildBones(maxBoneIndex, childrenBones);
			childrenBones.Add(_refCloth->GetBoneIndex(_refCloth->GetParentBone(maxBoneName)));
			bool samp = false;
			int maxChildIndex = 0;
			float maxChildVal = 0;
			//UE_LOG(LogTemp, Warning, TEXT("@ true %d"), childrenBones.Num());
			for (int j = 0; j < childrenBones.Num(); j++)
			{
				_col = FColor::Red;
				FName childBoneName = _refCloth->GetBoneName(childrenBones[j]);
				FVector childBonePosition = _refCloth->GetOwner()->GetTransform().TransformPosition(_refCloth->GetBoneLocation(childBoneName, EBoneSpaces::ComponentSpace));
				//FVector vec1 = _vertices[i].Position - maxBonePosition;
				FVector vec1 = vert - maxBonePosition;
				FVector vec2 = childBonePosition - maxBonePosition;
				//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), childBonePosition, 10, FColor::Cyan, true, 1, 0);

				FVector vec3 = vec1.ProjectOnTo(vec2);
				if (j == 0) {
					maxChildVal = FVector::DotProduct(vec2, vec3);
				}
				else if (maxChildVal < FVector::DotProduct(vec2, vec3)) {
					maxChildVal = FVector::DotProduct(vec2, vec3);
					maxChildIndex = j;
				}
				//|| j == childrenBones.Num() - 1
				if (FVector::DotProduct(vec2, vec3) > 0) {
					FName childBoneName = _refCloth->GetBoneName(childrenBones[maxChildIndex]);
					FVector childBonePosition = _refCloth->GetOwner()->GetTransform().TransformPosition(_refCloth->GetBoneLocation(childBoneName, EBoneSpaces::ComponentSpace));
					//FVector vec1 = _vertices[i].Position - maxBonePosition;
					FVector vec1 = vert - maxBonePosition;
					FVector vec2 = childBonePosition - maxBonePosition;
					//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), childBonePosition, 10, FColor::Cyan, true, 1, 0);

					FVector vec3 = vec1.ProjectOnTo(vec2);
					_col = FColor::Green;
					TArray<FHitResult> _resHit;
					FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(true);
					RV_TraceParams.bTraceComplex = true;
					//FVector _dir = (_vertices[i].Position - (maxBonePosition + vec3));
					FVector _dir = (vert - (maxBonePosition + vec3));
					_refAvatar->GetOwner()->GetWorld()->LineTraceMultiByChannel(_resHit,
						maxBonePosition + vec3 + _dir * 10,
						maxBonePosition + vec3,
						ECC_Visibility, RV_TraceParams);
					/*DrawDebugLine(_refCloth->GetOwner()->GetWorld(), maxBonePosition + vec3,
						maxBonePosition + vec3 + _dir * 2, FColor::Green, true, 1, 0, 0.1);*/
					bool collided = false;
					FVector hitVector;
					//UE_LOG(LogTemp, Warning, TEXT("@ _resHit %d"), _resHit.Num());

					for (int k = 0; k < _resHit.Num(); k++)
					{
						FHitResult _cHit = _resHit[k];
						if (_cHit.Component == _refAvatar) {
							//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
								//_cHit.Location,
								//12, FColor::Yellow, true, 1, 0);
							hitVector = _cHit.Location - (maxBonePosition + vec3);
							_col = FColor::Blue;
							samp = true;
							break;
						}
					}
					//UE_LOG(LogTemp, Warning, TEXT("@ %f :: %f"), hitVector.Size(), _dir.Size());
					for (int k = 0; k < _resHit.Num(); k++)
					{
						FHitResult _cHit = _resHit[k];
						if (_col == FColor::Blue && _cHit.Component == _targetAvatar) {
							//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
								//_cHit.Location,
								//12, FColor::Blue, true, 1, 0);
							//hitVector = _cHit.Location - (maxBonePosition + vec3);


							//_vertices[i].Position = (_cHit.Location) + (_dir - hitVector) + (_dir - hitVector).GetSafeNormal() * _amount;
							/*DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
								_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i),
								12, FColor::Red, true, 0.5, 0); */
								//FVector curHitVec = _cHit.Location - (maxBonePosition + vec3);
							vert = (_cHit.Location) + (_dir - hitVector)
								+ (_dir - hitVector).GetSafeNormal() * (_amount);

							/*DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
								_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i),
								12, FColor::Cyan, true, 0.5, 0);*/
							_col = FColor::Cyan;

							break;
						}
					}
					break;
				}
			}
			//if (_col == FColor::Red) {
			//	UE_LOG(LogTemp, Warning, TEXT("# RED %s"), *maxBoneName.ToString());
			//	for (int j = 0; j < childrenBones.Num(); j++)
			//	{
			//		_col = FColor::Red;
			//		FName childBoneName = _refCloth->GetBoneName(childrenBones[j]);
			//		FVector childBonePosition = _refCloth->GetOwner()->GetTransform().TransformPosition(_refCloth->GetBoneLocation(childBoneName, EBoneSpaces::ComponentSpace));
			//		//FVector vec1 = _vertices[i].Position - maxBonePosition;
			//		FVector vec1 = vert - maxBonePosition;
			//		FVector vec2 = childBonePosition - maxBonePosition;
			//		//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), childBonePosition, 10, FColor::Cyan, true, 1, 0);

			//		FVector vec3 = vec1.ProjectOnTo(vec2);
			//		UE_LOG(LogTemp, Warning, TEXT("# %f"), FVector::DotProduct(vec2, vec3));
			//		//if (FVector::DotProduct(vec2, vec3) > 0)
			//	}
			//}

			//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), _vertices[i].Position, 12, _col, true, 0.5, 0);
			if (_col != FColor::Cyan) {
				//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), vert, 12, _col, true, 0.5, 0);

				FName childBoneName = _refCloth->GetParentBone(maxBoneName);
				FVector childBonePosition = _refCloth->GetOwner()->GetTransform().TransformPosition(_refCloth->GetBoneLocation(childBoneName, EBoneSpaces::ComponentSpace));
				//FVector vec1 = _vertices[i].Position - maxBonePosition;
				FVector vec1 = vert - maxBonePosition;
				FVector vec2 = childBonePosition - maxBonePosition;
				//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), childBonePosition, 10, FColor::Cyan, true, 1, 0);

				FVector vec3 = vec1.ProjectOnTo(vec2);
				_col = FColor::Green;
				TArray<FHitResult> _resHit;
				FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(true);
				RV_TraceParams.bTraceComplex = true;
				//FVector _dir = (_vertices[i].Position - (maxBonePosition + vec3));
				FVector _dir = (vert - (maxBonePosition + vec3));
				/*_refAvatar->GetOwner()->GetWorld()->LineTraceMultiByChannel(_resHit,
					maxBonePosition + vec3 + _dir * 10,
					maxBonePosition + vec3,
					ECC_Visibility, RV_TraceParams);*/
				UE_LOG(LogTemp, Warning, TEXT("! Bonename: %s :: %s"), *maxBoneName.ToString(), *childBoneName.ToString());
				/*DrawDebugLine(_refCloth->GetOwner()->GetWorld(), maxBonePosition + vec3,
					maxBonePosition + vec3 + _dir * 10, FColor::Black, true, 1, 0, 0.5);*/
				_refAvatar->GetOwner()->GetWorld()->LineTraceMultiByChannel(_resHit,
					maxBonePosition + vec3 + _dir * 10,
					maxBonePosition + vec3,
					ECC_Visibility, RV_TraceParams);
				/*DrawDebugLine(_refCloth->GetOwner()->GetWorld(), maxBonePosition + vec3,
					maxBonePosition + vec3 + _dir * 2, FColor::Green, true, 1, 0, 0.1);*/
				bool collided = false;
				FVector hitVector;
				//UE_LOG(LogTemp, Warning, TEXT("@ _resHit %d"), _resHit.Num());

				for (int k = 0; k < _resHit.Num(); k++)
				{
					FHitResult _cHit = _resHit[k];
					if (_cHit.Component == _refAvatar) {
						//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
							//_cHit.Location,
							//12, FColor::Yellow, true, 1, 0);
						hitVector = _cHit.Location - (maxBonePosition + vec3);
						_col = FColor::Blue;
						samp = true;
						break;
					}
				}
				//UE_LOG(LogTemp, Warning, TEXT("@ %f :: %f"), hitVector.Size(), _dir.Size());
				for (int k = 0; k < _resHit.Num(); k++)
				{
					FHitResult _cHit = _resHit[k];
					if (_col == FColor::Blue && _cHit.Component == _targetAvatar) {
						//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
							//_cHit.Location,
							//12, FColor::Blue, true, 1, 0);
						//hitVector = _cHit.Location - (maxBonePosition + vec3);


						//_vertices[i].Position = (_cHit.Location) + (_dir - hitVector) + (_dir - hitVector).GetSafeNormal() * _amount;
						/*DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
							_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i),
							12, FColor::Red, true, 0.5, 0); */
							//FVector curHitVec = _cHit.Location - (maxBonePosition + vec3);
						vert = (_cHit.Location) + (_dir - hitVector)
							+ (_dir - hitVector).GetSafeNormal() * (_amount);

						/*DrawDebugPoint(_refCloth->GetOwner()->GetWorld(),
							_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i),
							12, FColor::Cyan, true, 0.5, 0);*/
						_col = FColor::Cyan;

						break;
					}
				}
			}
			/*if (_col != FColor::Cyan) {
				DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), vert, 12, _col, true, 0.5, 0);
			}*/
			//if (!samp) {
			//	DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), _vertices[i].Position, 12, FColor::Green, true, 1, 0);

			//}
			//else {
			//	//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), _vertices[i].Position, 12, FColor::Orange, true, 1, 0);

			//}
		}
	}
	//}

	//_mesh->PostEditChange();
	_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.InitRHI();
	_targetCloth->GetOwner()->MarkComponentsRenderStateDirty();

	return _oldVerts;

}

TArray<TArray<int32>> UMyBlueprintFunctionLibrary::FindChildrenBones(USkeletalMeshComponent * _meshComp) {
	TArray<TArray<int32>> result = TArray<TArray<int32>>();
	for (int i = 0; i < _meshComp->GetNumBones(); i++)
	{
		result.Add(TArray<int32>());
	}

	for (int i = 0; i < _meshComp->GetNumBones(); i++)
	{
		int _parent = _meshComp->GetBoneIndex(_meshComp->GetParentBone(_meshComp->GetBoneName(i)));
		if (_parent >= 0 && _parent < _meshComp->GetNumBones())
			result[_parent].Add(i);
		/*if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 50, FColor::Red, TEXT("FindChildrenBones ")
			+ FString::FromInt(_parent));*/
			//result.Add(TArray<int32>());
	}

	return result;
}

FName UMyBlueprintFunctionLibrary::GetRelevantBone(UPoseableMeshComponent * _mesh, FName _bone)
{
	TArray<FName> ignoreBoneRoot = { FName("hand_r") , FName("hand_l") , FName("foot_l") , FName("foot_r") ,
		FName("ik_foot_root") , FName("ik_hand_root") };
	//UE_LOG(LogTemp, Warning, TEXT("New1 %d"), _boneIndex);

	FName currentBone = FName(_bone);
	//UE_LOG(LogTemp, Warning, TEXT("New2 %s"), *currentBone.ToString());


	FName resultBone = FName(currentBone);

	while (currentBone.Compare(FName("None")) != 0)
	{
		for (int i = 0; i < ignoreBoneRoot.Num(); i++)
		{
			if (currentBone.Compare(ignoreBoneRoot[i]) == 0) {
				if (i == 4 || i == 5)
					return _mesh->GetParentBone(currentBone);
				return currentBone;
			}
		}

		if (currentBone.ToString().Contains("twist")) {
			return _mesh->GetParentBone(currentBone);
		}
		currentBone = _mesh->GetParentBone(currentBone);
	}
	return resultBone;
}

void UMyBlueprintFunctionLibrary::ResetVertices(USkeletalMeshComponent * _meshComp, TArray<FVector> _oldVertices)
{
	USkeletalMesh *_mesh = _meshComp->SkeletalMesh;
	for (int i = 0; i < (int)_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.GetNumVertices() && i < _oldVertices.Num(); i++)
	{
		//DrawDebugPoint(_refCloth->GetOwner()->GetWorld(), _vertices[i].Position, 12, FColor::Black, true, 1, 0);
		FVector &vert = _mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i);
		vert = _oldVertices[i];
	}
	_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.InitRHI();
	_meshComp->GetOwner()->MarkComponentsRenderStateDirty();

	//FSkeletalMeshLODModel& LODModel = _mesh->GetImportedModel()->LODModels[0];
	//int _mainInd = 0;
	//for (int sec = 0; sec < _mesh->GetImportedModel()->LODModels[0].Sections.Num(); sec++)
	//{
	//	TArray<FSoftSkinVertex> &vertices = LODModel.Sections[sec].SoftVertices;
	//	for (int32 i = 0; i < vertices.Num(); i++, _mainInd++)
	//	{
	//		vertices[i].Position = _oldVertices[_mainInd];
	//	}
	//}
	//TArray<FSoftSkinVertex> &vertices = LODModel.Sections[0].SoftVertices;
	//for (int32 i = 0; i < FMath::Min(vertices.Num(), _oldVertices.Num()); i++)
	//{
	//	vertices[i].Position = _oldVertices[i];
	//}
	//_mesh->PostEditChange();

}

void UMyBlueprintFunctionLibrary::IncreaseIndicesCapacity1(USkeletalMeshComponent * _refAvatar)
{
	UE_LOG(LogTemp, Warning, TEXT("@ IncreaseIndicesCapacity1"));
	USkeletalMesh *_mesh = _refAvatar->SkeletalMesh;
	_refAvatar->SetVisibility(false);
	UMyBlueprintFunctionLibrary::_skelStatic = _refAvatar;
#if WITH_EDITOR
#else
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(_skelStatic, USkeletalMeshComponent*, USkeletalMeshComponent, _skelStatic,
		{

			UE_LOG(LogTemp, Warning, TEXT("@! IncreaseIndicesCapacity MultiSizeIndexContainer ReleaseResource"));
			UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->ReleaseResource();

		});
#endif
}
void UMyBlueprintFunctionLibrary::IncreaseIndicesCapacity2(USkeletalMeshComponent * _refAvatar) {
	UE_LOG(LogTemp, Warning, TEXT("@ IncreaseIndicesCapacity2"));
	USkeletalMesh *_mesh = _refAvatar->SkeletalMesh;

	TArray<uint32> indBuffer = TArray<uint32>();
	for (int32 i = 0; i < _mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->Num(); i++)
	{
		indBuffer.Add(_mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->Get(i));
	}

	UE_LOG(LogTemp, Warning, TEXT("@! IncreaseIndicesCapacity MultiSizeIndexContainer RebuildIndexBuffer 1: %d"), _mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetDataTypeSize());
	_mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.RebuildIndexBuffer(sizeof(uint32), indBuffer);
	UE_LOG(LogTemp, Warning, TEXT("@! IncreaseIndicesCapacity MultiSizeIndexContainer RebuildIndexBuffer 2: %d"), _mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetDataTypeSize());
#if WITH_EDITOR
#else
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(_skelStatic, USkeletalMeshComponent*, USkeletalMeshComponent, _skelStatic,
		{
		UE_LOG(LogTemp, Warning, TEXT("@IncreaseIndicesCapacity MultiSizeIndexContainer InitResource"));
		UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->InitResource();

		//UE_LOG(LogTemp, Warning, TEXT("@IncreaseIndicesCapacity MultiSizeIndexContainer ReleaseRHI"));
		//UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->ReleaseRHI();
		//UE_LOG(LogTemp, Warning, TEXT("@IncreaseIndicesCapacity MultiSizeIndexContainer InitRHI"));
		//UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->InitRHI();
		//UE_LOG(LogTemp, Warning, TEXT("@IncreaseIndicesCapacity MultiSizeIndexContainer InitDynamicRHI"));
		//UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->InitDynamicRHI();
		}

	);
#endif
	//UE_LOG(LogTemp, Warning, TEXT("@! SetVisibility(true)"));
	//_refAvatar->SetVisibility(true);

}

void UMyBlueprintFunctionLibrary::LoadSkeletalMesh(USkeletalMeshComponent * _refAvatar, FReturnedData _importedData)
{
	UE_LOG(LogTemp, Warning, TEXT("@ LoadSkeletalMesh"));

	USkeletalMesh *_mesh = _refAvatar->SkeletalMesh;
	UMyBlueprintFunctionLibrary::oldVerticesNum = (int)_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.GetNumVertices();
	if (!_importedData.bSuccess) {
		UE_LOG(LogTemp, Warning, TEXT("@ Failed at 1"));
		return;
	}
	TArray<int> bones = TArray<int>();

	UE_LOG(LogTemp, Warning, TEXT("@ _refAvatar->GetNumBones(): %d"), _refAvatar->GetNumBones());
	for (int i = 0; i < _refAvatar->GetNumBones(); i++) {
		bones.AddUnique(_refAvatar->GetBoneIndex(UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, _refAvatar->GetBoneName(i))));
	}

	UE_LOG(LogTemp, Warning, TEXT("@ _refAvatar->HasExtraBoneInfluences(): %d"), _mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer.HasExtraBoneInfluences());

	//TMap<int32, int32> SkelToSectionBoneMap;
	//for (int32 i = 0; i < _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap.Num(); i++)
	//{
	//	SkelToSectionBoneMap.Add(_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap[i], i);
	//	UE_LOG(LogTemp, Warning, TEXT("@ SkelToSectionBoneMap Key: %d"), _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap[i]);
	//}
	//UE_LOG(LogTemp, Warning, TEXT("@ SkelToSectionBoneMap(Added 0, 0)"));
	//SkelToSectionBoneMap.Add(0, 0);
	//UE_LOG(LogTemp, Warning, TEXT("@ SkelToSectionBoneMap Length: %d"), SkelToSectionBoneMap.Num());
	UE_LOG(LogTemp, Warning, TEXT("@ boneMap before %d"), _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap.Num());
	int numOfBones = _refAvatar->GetNumBones();

	//TArray<int> tempBoneMap = TArray<int>();
	//for (int32 i = 0; i < _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap.Num(); i++) {
	//	tempBoneMap.Add(_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap[i]);
	//}

	//for (int32 i = _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap.Num(); i < _refAvatar->GetNumBones(); i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("@ boneMap %d"), i);
	//	UE_LOG(LogTemp, Warning, TEXT("@ boneMap name: %s"), *_refAvatar->GetBoneName(i).ToString());
	//	UE_LOG(LogTemp, Warning, TEXT("@ boneMap rel name: %s"), *UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, _refAvatar->GetBoneName(i)).ToString());
	//	UE_LOG(LogTemp, Warning, TEXT("@ boneMap rel index: %d"), _refAvatar->GetBoneIndex(UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, _refAvatar->GetBoneName(i))));

	//	tempBoneMap.Add(
	//		SkelToSectionBoneMap[
	//			_refAvatar->GetBoneIndex(UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, _refAvatar->GetBoneName(i)))
	//		]
	//	);
	//}
	_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap.Empty();
	for (int32 i = 0; i < numOfBones; i++) {
		_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap.Add(i);
	}

	UE_LOG(LogTemp, Warning, TEXT("@ boneMap after %d"), _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap.Num());


	TArray<FVector> finalVertices = TArray<FVector>();
	TArray<FProcMeshTangent> finalTangents = TArray<FProcMeshTangent>();
	TArray<FVector> finalNormals = TArray<FVector>();

	for (int i = 0; i < _importedData.meshInfo.Num(); i++)
	{
		finalVertices.Append(_importedData.meshInfo[i].Vertices);
		finalTangents.Append(_importedData.meshInfo[i].Tangents);
		finalNormals.Append(_importedData.meshInfo[i].Normals);
		TArray<FString> _keys = TArray<FString>();
		_importedData.meshInfo[i].BoneInfo.GetKeys(_keys);
		//UE_LOG(LogTemp, Warning, TEXT("@ Mesh: %d"), i);
		for (int j = 0; j < _importedData.meshInfo[i].BoneInfo.Num(); j++) {
			//UE_LOG(LogTemp, Warning, TEXT("@ boneName: %s"), *_keys[j]);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("@ oldVerticesNum: %d"), UMyBlueprintFunctionLibrary::oldVerticesNum);
	UE_LOG(LogTemp, Warning, TEXT("@ boneName: %d"), finalVertices.Num());
	for (int i = 0; i < finalVertices.Num(); i++) {
		if (i < UMyBlueprintFunctionLibrary::oldVerticesNum) {
			FVector &v = _mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i);
			v = finalVertices[i].RotateAngleAxis(180, FVector(1, 0, 0));
			_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i,
				(finalTangents[i].TangentX).RotateAngleAxis(180, FVector(1, 0, 0)),
				(FVector::CrossProduct(finalNormals[i], finalTangents[i].TangentX).RotateAngleAxis(180, FVector(1, 0, 0))),
				(finalNormals[i]).RotateAngleAxis(180, FVector(1, 0, 0)));
		}
		else {
			FStaticMeshBuildVertex a1;
			a1.Position = finalVertices[i].RotateAngleAxis(180, FVector(1, 0, 0));
			_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.AppendVertices(&a1, 1);

			a1.TangentX = (finalTangents[i].TangentX).RotateAngleAxis(180, FVector(1, 0, 0));
			a1.TangentY = (FVector::CrossProduct(finalNormals[i], finalTangents[i].TangentX)).RotateAngleAxis(180, FVector(1, 0, 0));
			a1.TangentZ = (finalNormals[i]).RotateAngleAxis(180, FVector(1, 0, 0));

			for (int j = 0; j < (int)_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.StaticMeshVertexBuffer.GetNumTexCoords(); j++) {
				a1.UVs[j] = _mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(0, j);
			}

			_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.StaticMeshVertexBuffer.AppendVertices(&a1, 1);
		}
	}

	TArray<TSkinWeightInfo<true>> InWeights;

	InWeights.AddUninitialized(_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.GetNumVertices());
	for (int i = 0; i < (int)_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.GetNumVertices(); i++) {

		FVector &v = _mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i);

		//int boneInd = _refAvatar->GetBoneIndex(UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, _refAvatar->FindClosestBone(_refAvatar->GetOwner()->GetTransform().TransformPosition(v))));
		int boneInd = _refAvatar->GetBoneIndex(FName("spine_02"));
		//boneInd = SkelToSectionBoneMap[boneInd];
		InWeights[i].InfluenceWeights[0] = (uint8_t)(255);
		InWeights[i].InfluenceBones[0] = (uint8_t)boneInd;
		for (uint32_t BlendIndex = 1; BlendIndex < 8; BlendIndex++)
		{
			InWeights[i].InfluenceWeights[BlendIndex] = (uint8_t)0;
			InWeights[i].InfluenceBones[BlendIndex] = (uint8_t)0;
		}
	}
	//_mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer.SetHasExtraBoneInfluences(false);
	//_mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer = InWeights;


	//TArray<TSkinWeightInfo<false>> InWeights;
	TArray<int> _weightIndices = TArray<int>();
	_weightIndices.AddZeroed(_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.GetNumVertices());

	//---------------------------
	UE_LOG(LogTemp, Warning, TEXT("@ BoneInfo"));

	int _vertIndexOffset = 0;
	for (int i = 0; i < _importedData.meshInfo.Num(); i++)
	{
		TArray<FString> _boneNames;
		_importedData.meshInfo[i].BoneInfo.GetKeys(_boneNames);
		//UE_LOG(LogTemp, Warning, TEXT("$ submesh: %d"), i);
		//UE_LOG(LogTemp, Warning, TEXT("$ _boneNames.Num(): %d"), _boneNames.Num());
		for (int j = 0; j < _boneNames.Num(); j++) {
			//UE_LOG(LogTemp, Warning, TEXT("--------------------------- \n--------------------------- \n "));
			//UE_LOG(LogTemp, Warning, TEXT("$$boneName: %s"), *_boneNames[j]);
			//UE_LOG(LogTemp, Warning, TEXT("$$Rel boneName: %s"), *UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, FName(*_boneNames[j])).ToString());
			//UE_LOG(LogTemp, Warning, TEXT("$$Rel boneInd: %d"), (uint8_t)_refAvatar->GetBoneIndex(UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, FName(*_boneNames[j]))));
			//if (_boneNames[j].Compare("lowerarm_l") != 0) {
			//	continue;
			//}
			TArray<TTuple<int32, float>> _weights = _importedData.meshInfo[i].BoneInfo[_boneNames[j]];
			for (int k = 0; k < _weights.Num(); k++) {
				//UE_LOG(LogTemp, Warning, TEXT("--------------------------- \n $$vertNum: %d"), (_vertIndexOffset + _weights[k].Key));
				//UE_LOG(LogTemp, Warning, TEXT("$$weight: %d"), (uint8_t)(_weights[k].Value * 255.0));
				//UE_LOG(LogTemp, Warning, TEXT("$$boneIndex: %d"), (uint8_t)(_weights[k].Value * 255.0));

				if (_weightIndices[_vertIndexOffset + _weights[k].Key] >= 8) {
					UE_LOG(LogTemp, Warning, TEXT("!!!! error BoneInfo: %d"), (_vertIndexOffset + _weights[k].Key));
					continue;
				}
				InWeights[_vertIndexOffset + _weights[k].Key].InfluenceWeights[_weightIndices[_vertIndexOffset + _weights[k].Key]] = (uint8_t)(_weights[k].Value * 255.0);
				//InWeights[_vertIndexOffset + _weights[k].Key].InfluenceWeights[_weightIndices[_vertIndexOffset + _weights[k].Key]] = (uint8_t)(255);
				InWeights[_vertIndexOffset + _weights[k].Key].InfluenceBones[_weightIndices[_vertIndexOffset + _weights[k].Key]] = (uint8_t)_refAvatar->GetBoneIndex(UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, FName(*_boneNames[j])));
				//UE_LOG(LogTemp, Warning, TEXT("^ 5 _boneName: %d"), k);
				if (UMyBlueprintFunctionLibrary::GetRelevantBone(_refAvatar, FName(*_boneNames[j])).Compare(FName("None")) == 0) {
					UE_LOG(LogTemp, Warning, TEXT("$ GetRelevantBone Bone not found: %s"), *_boneNames[j]);
				}
				_weightIndices[_vertIndexOffset + _weights[k].Key] += 1;
			}
		}
		_vertIndexOffset += _importedData.meshInfo[i].Vertices.Num();

	}

	//for (int i = 0; i < _weightIndices.Num(); i++) {
	//	//if (_weightIndices[i] > 4) {
	//	//	UE_LOG(LogTemp, Warning, TEXT("$ _weightIndices Greater: %d"), i);

	//	//}
	//	if (_weightIndices[i] == 0) {
	//		InWeights[i].InfluenceWeights[0] = (uint8_t)(255);
	//		InWeights[i].InfluenceBones[0] = (uint8_t)0;				
	//	}
	//}

	_mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer.SetHasExtraBoneInfluences(true);
	_mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer = InWeights;


	UE_LOG(LogTemp, Warning, TEXT("@ PositionVertexBuffer.InitRHI()"));
	_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.InitRHI();
	UE_LOG(LogTemp, Warning, TEXT("@ StaticMeshVertexBuffer.InitRHI()"));
	_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.StaticMeshVertexBuffer.InitRHI();
	UE_LOG(LogTemp, Warning, TEXT("@ SkinWeightVertexBuffer.InitRHI()"));
	_mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer.InitRHI();

	UE_LOG(LogTemp, Warning, TEXT("@ PositionVertexBuffer.InitDynamicRHI()"));
	_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.PositionVertexBuffer.InitDynamicRHI();
	UE_LOG(LogTemp, Warning, TEXT("@ StaticMeshVertexBuffer.InitDynamicRHI()"));
	_mesh->GetResourceForRendering()->LODRenderData[0].StaticVertexBuffers.StaticMeshVertexBuffer.InitDynamicRHI();
	UE_LOG(LogTemp, Warning, TEXT("@ SkinWeightVertexBuffer.InitDynamicRHI()"));
	_mesh->GetResourceForRendering()->LODRenderData[0].SkinWeightVertexBuffer.InitDynamicRHI();

	UMyBlueprintFunctionLibrary::_skelStatic = _refAvatar;

	//Triangles
	UE_LOG(LogTemp, Warning, TEXT("@ Triangles"));

	int startIndex = UMyBlueprintFunctionLibrary::oldVerticesNum;
	UMyBlueprintFunctionLibrary::oldIndexBuffer2 = TArray<uint32>();

	int baseVertexIndex = 0;
	for (int i = 0; i < _importedData.meshInfo.Num(); i++)
	{
		for (int j = 0; j < _importedData.meshInfo[i].Triangles.Num(); j++) {
			UMyBlueprintFunctionLibrary::oldIndexBuffer2.Add(baseVertexIndex + _importedData.meshInfo[i].Triangles[j]);
		}
		baseVertexIndex += _importedData.meshInfo[i].Vertices.Num();
	}

	for (int i = 0; i < _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections.Num(); i++) {
		if (i < _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections.Num() - 1) {
			_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[i].BaseIndex = 0;
			_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[i].BaseVertexIndex = 0;
			_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[i].NumTriangles = 1;
			_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[i].BoneMap = _mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[3].BoneMap;
		}
		else {
			_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[i].BaseIndex = 0;
			_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[i].BaseVertexIndex = 0;
			_mesh->GetResourceForRendering()->LODRenderData[0].RenderSections[i].NumTriangles = UMyBlueprintFunctionLibrary::oldIndexBuffer2.Num() / 3;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CopyIndexBuffer 1 %d"), _mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->Num());
	_mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.CopyIndexBuffer(UMyBlueprintFunctionLibrary::oldIndexBuffer2);
	UE_LOG(LogTemp, Warning, TEXT("CopyIndexBuffer 2 %d"), _mesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->Num());
	UE_LOG(LogTemp, Warning, TEXT("CopyIndexBuffer 3 %d"), UMyBlueprintFunctionLibrary::oldIndexBuffer2.Num());

	UMyBlueprintFunctionLibrary::_skelStatic = _refAvatar;
	_mesh->ReleaseResources();
	_mesh->InitResources();

#if WITH_EDITOR
#else
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(_skelStatic, USkeletalMeshComponent*, USkeletalMeshComponent, _skelStatic,
		{
		UE_LOG(LogTemp, Warning, TEXT("@MultiSizeIndexContainer ReleaseRHI"));
		UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->ReleaseRHI();
		UE_LOG(LogTemp, Warning, TEXT("@MultiSizeIndexContainer InitRHI"));
		UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->InitRHI();
		UE_LOG(LogTemp, Warning, TEXT("@MultiSizeIndexContainer InitDynamicRHI"));
		UMyBlueprintFunctionLibrary::_skelStatic->SkeletalMesh->GetResourceForRendering()->LODRenderData[0].MultiSizeIndexContainer.GetIndexBuffer()->InitDynamicRHI();
		}
	);
#endif
}

FName UMyBlueprintFunctionLibrary::GetRelevantBone(USkeletalMeshComponent * _mesh, FName _bone)
{
	TArray<FName> ignoreBoneRoot = { FName("hand_r") , FName("hand_l") , FName("foot_l") , FName("foot_r") ,
		FName("ik_foot_root") , FName("ik_hand_root") };
	//UE_LOG(LogTemp, Warning, TEXT("New1 %d"), _boneIndex);
	//UE_LOG(LogTemp, Warning, TEXT("GetRelevantBone 1 %s"), *_bone.ToString());

	FName currentBone = FName(_bone);
	//UE_LOG(LogTemp, Warning, TEXT("New2 %s"), *currentBone.ToString());


	FName resultBone = FName(currentBone);

	while (currentBone.Compare(FName("None")) != 0)
	{
		for (int i = 0; i < ignoreBoneRoot.Num(); i++)
		{
			if (currentBone.Compare(ignoreBoneRoot[i]) == 0) {
				if (i == 4 || i == 5) {
					//UE_LOG(LogTemp, Warning, TEXT("GetRelevantBone i == 4 || i == 5 %s"), *_mesh->GetParentBone(currentBone).ToString());
					return _mesh->GetParentBone(currentBone);

				}
				//UE_LOG(LogTemp, Warning, TEXT("GetRelevantBone 3 %s"), *currentBone.ToString());
				return currentBone;
			}
		}

		if (currentBone.Compare(FName("lowerarm_twist_01_l")) == 0) {
			return FName("lowerarm_l");
		}
		if (currentBone.Compare(FName("lowerarm_twist_01_r")) == 0) {
			return FName("lowerarm_r");
		}
		if (currentBone.Compare(FName("upperarm_twist_01_l")) == 0) {
			return FName("upperarm_l");
		}
		if (currentBone.Compare(FName("upperarm_twist_01_r")) == 0) {
			return FName("upperarm_r");
		}
		if (currentBone.Compare(FName("calf_twist_01_r")) == 0) {
			return FName("calf_r");
		}
		if (currentBone.Compare(FName("calf_twist_01_l")) == 0) {
			return FName("calf_l");
		}
		if (currentBone.Compare(FName("thigh_twist_01_r")) == 0) {
			return FName("thigh_r");
		}
		if (currentBone.Compare(FName("thigh_twist_01_l")) == 0) {
			return FName("thigh_l");
		}
		if (currentBone.ToString().Contains("twist")) {
			//if (currentBone.ToString().Contains("upperarm")) {
			//	if(currentBone.ToString().Contains("_r"))
			//		return FName("upperarm_r");
			//	else
			//		return FName("upperarm_l");


			//}
			//UE_LOG(LogTemp, Warning, TEXT("GetRelevantBone twist %s"), *_mesh->GetParentBone(currentBone).ToString());

			return _mesh->GetParentBone(currentBone);
		}
		currentBone = _mesh->GetParentBone(currentBone);
		//UE_LOG(LogTemp, Warning, TEXT("GetRelevantBone 2 %s"), *currentBone.ToString());
	}
	return resultBone;
}


int UMyBlueprintFunctionLibrary::GetNearestBone(USkeletalMeshComponent * _mesh, TArray<int> _fromBones, FVector _pos) {
	float minDist;
	int result;
	for (int i = 0; i < _fromBones.Num(); i++) {
		if (i == 0) {
			minDist = FVector::Dist(_mesh->GetBoneLocation(_mesh->GetBoneName(_fromBones[i]), EBoneSpaces::ComponentSpace), _pos);
			result = 0;
		}
		else {
			float currentDist = FVector::Dist(_mesh->GetBoneLocation(_mesh->GetBoneName(_fromBones[i]), EBoneSpaces::ComponentSpace), _pos);
			if (minDist > currentDist) {
				minDist = currentDist;
				result = i;
			}
		}
	}

	return _fromBones[result];
}
