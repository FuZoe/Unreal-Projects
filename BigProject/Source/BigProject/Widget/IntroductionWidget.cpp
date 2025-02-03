#include "IntroductionWidget.h"
#include "Components/Button.h"

void UIntroductionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OKButton->OnClicked.AddDynamic(this, &UIntroductionWidget::OnButtonClicked);
}

void UIntroductionWidget::OnButtonClicked()
{
	APlayerController* Controller =  GetOwningPlayer();
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->SetShowMouseCursor(false);
	this->RemoveFromParent();
}
