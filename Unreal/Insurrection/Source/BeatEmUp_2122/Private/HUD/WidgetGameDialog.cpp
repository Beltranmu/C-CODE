#include "HUD/WidgetGameDialog.h"
#include "Kismet/GameplayStatics.h"
#include "Core/GameInstanceBEU.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "HUD/WidgetCustomButton.h"
#include "GameFramework/HUD.h"

void UWidgetGameDialog::NativeConstruct()
{

	Super::NativeConstruct();

	gameInstance = Cast<UGameInstanceBEU>(UGameplayStatics::GetGameInstance(GetWorld()));

	gameInstance->nDialog = 1;
	gameInstance->nImage = 1;

	nextDialog->baseButton->OnClicked.AddDynamic(this, &UWidgetGameDialog::triggerNextDialog);

}

void UWidgetGameDialog::triggerNextDialog()
{

	gameInstance->nDialog++;

}

void UWidgetGameDialog::setFocusToButton()
{

	nextDialog->baseButton->SetFocus();

}

void UWidgetGameDialog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	Super::NativeTick(MyGeometry, InDeltaTime);

	//MOVER ESTO EN UN FUTURO AL ADDYNAMIC DEL BOTON || IMPORTANTE || SE INTENTO UN DIA Y PETABA, IWAL SE QUEDA AQUI

	FDialogData* DataRow = dialogData->FindRow<FDialogData>(FName(FString::FromInt(gameInstance->nDialog)), TEXT(""));

	if (DataRow)
	{

		dialogToDisplay->SetText(DataRow->DisplayDialog);

		if (DataRow->ImgSpeaker1 == nullptr)
		{

			imageLeftDialog->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 0.0));

		}
		else
		{

			imageLeftDialog->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 1.0));

		}

		if (DataRow->ImgSpeaker2 == nullptr)
		{

			imageRightDialog->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 0.0));

		}
		else
		{

			imageRightDialog->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 1.0));

		}

		imageLeftDialog->SetBrushFromTexture(DataRow->ImgSpeaker1);
		imageRightDialog->SetBrushFromTexture(DataRow->ImgSpeaker2);
		
		imageLeftDialog->Slot->Content->SetRenderScale(FVector2D(DataRow->sizeMultiplierSpeaker1X, DataRow->sizeMultiplierSpeaker1Y));
		imageRightDialog->Slot->Content->SetRenderScale(FVector2D(DataRow->sizeMultiplierSpeaker2X, DataRow->sizeMultiplierSpeaker2Y));
		
		//imageLeftDialog->Brush.SetImageSize(FVector2D(10.0f, 10.0f));
		//imageRightDialog->Brush.SetImageSize(FVector2D(10.0f, 10.0f));
		//imageLeftDialog->SetBrushSize(FVector2D(10.0f, 10.0f));
		//imageRightDialog->SetBrushSize(FVector2D(10.0f, 10.0f));


		//imageLeftDialog->SetBrushSize(FVector2D(10.0f, 10.0f));

	}
	else
	{
		
		return;

	}

	//FImageDataRow* ImgRow = ImageData->FindRow<FImageDataRow>(FName(FString::FromInt(gameInstance->nImage)), TEXT(""));
	//
	//if (ImgRow)
	//{
	//	imageLeftDialog->SetBrushFromTexture(ImgRow->Icon);
	//}

	//https://www.orfeasel.com/consuming-data-tables/
	//imageLeftDialog->SetBrushSize(FVector2D(10.0f, 10.0f));																	CHANGE IMAGE SIZE TO DESIRE SIZE
	//imageLeftDialog->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));								MAKE DISSAPEAR IMAGE IF NEEDED

}
