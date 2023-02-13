// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Framework/Resources.h"

UResources::UResources() {

  type_ = -1;
  amount_ = 0;

}

UResources* UResources::Init() {

  UResources* auxResource = nullptr;
  auxResource->SetType(-1);
  auxResource->SetAmount(0);

  return auxResource;
}

UResources* UResources::Init(const int type, const float value) {

  UResources* aux = nullptr;

  aux->type_ = type;
  aux->amount_ = value;

  return aux;
}

void UResources::SetType(int value) {

  type_ = value;

}

int UResources::Type() {

  return type_;

}

void UResources::SetAmount(float value) {

  amount_ += value;

}

void UResources::SetAmountValue(float value) {

  amount_ = value;

}

float UResources::Amount() {

  return amount_;

}
