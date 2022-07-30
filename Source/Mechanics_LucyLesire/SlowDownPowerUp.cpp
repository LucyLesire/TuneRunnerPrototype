// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowDownPowerUp.h"

// Sets default values
ASlowDownPowerUp::ASlowDownPowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		 // Set the sphere's collision profile name to "Projectile".
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// Event called when component overlaps with something.
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlowDownPowerUp::OnOverlapBegin);
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(32.f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 800.f;
		ProjectileMovementComponent->MaxSpeed = 800.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = false;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->Bounciness = 0.0f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		FireInDirection(FVector{ -1.f, 0.f, 0.f });

	}
	//Create mesh component
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}
	//Attach Material to object
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Blueprints/Materials/MAT_Beat_4.MAT_Beat_4'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	//Initialize MeshComponent
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeLocation(FVector{ 0.f,0.f,-50.f });
	ProjectileMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
	ProjectileMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	InitialLifeSpan = 5.0f;

}

// Called when the game starts or when spawned
void ASlowDownPowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlowDownPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//Fire projectile in a certain direction
void ASlowDownPowerUp::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

//Function that is called when the projectile hits something.
void ASlowDownPowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			//Call function on other actor
			FOutputDeviceNull ar;
			const FString command = FString::Printf(TEXT("Slowdowntimepowerup true"));
			OtherActor->CallFunctionByNameWithArguments(*command, ar, NULL, true);

			Destroy();
		}
	}

}
