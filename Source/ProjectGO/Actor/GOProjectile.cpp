// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGO/Actor/GOProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGOProjectile::AGOProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");

	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AGOProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGOProjectile::OnSphereOverlaped);
}

void AGOProjectile::OnSphereOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
