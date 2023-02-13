// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/UI/Inventorywidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorywidget::Init(){

  Slots.Add(Slot1);
  Slots.Add(Slot2);
  Slots.Add(Slot3);
  Slots.Add(Slot4);
  Slots.Add(Slot5);
  Slots.Add(Slot6);
  Slots.Add(Slot7);
  Slots.Add(Slot8);
  Slots.Add(Slot9);
  Slots.Add(Slot10);

  UImage* object_img;

  for (int i = 0; i < 10; ++i) {
    object_img = Cast<UImage>(Slots[i]->GetChildAt(0));
    object_img->SetOpacity(0.0f);
  }

}

void UInventorywidget::Clean() {
  UImage* object_img;
  UTextBlock* text;
  for (int i = 0; i < 10; ++i) {
    object_img = Cast<UImage>(Slots[i]->GetChildAt(0));
    text = Cast<UTextBlock>(Slots[i]->GetChildAt(1));
    object_img->SetOpacity(0.0f);

    FText empty_text = FText::FromString("");
    text->SetText(empty_text);
  }
}
