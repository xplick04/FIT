from rest_framework import serializers

from .models import (
	UserModel,
	AnimalModel,
	ScheduleModel,
	ReservationModel,
	RecordModel,
	RequestModel,
)

from rest_framework_simplejwt.serializers import TokenObtainPairSerializer

class MyTokenObtainPairSerializer(TokenObtainPairSerializer):
	@classmethod
	def get_token(cls, user):
		token = super().get_token(user)

		token['username'] = user.username
		token['type'] = user.type
		token['first_name'] = user.first_name
		token['verified'] = str(user.verified)

		return token

class UserSerializer(serializers.ModelSerializer):
	class Meta:
		model = UserModel
		fields = '__all__'

class AnimalSerializer(serializers.ModelSerializer):
	class Meta:
		model = AnimalModel
		fields = '__all__'

class ScheduleSerializer(serializers.ModelSerializer):
	class Meta:
		model = ScheduleModel
		fields = '__all__'

class ReservationSerializer(serializers.ModelSerializer):
	class Meta:
		model = ReservationModel
		fields = '__all__'

class RecordSerializer(serializers.ModelSerializer):
	class Meta:
		model = RecordModel
		fields = '__all__'

class RequestSerializer(serializers.ModelSerializer):
	class Meta:
		model = RequestModel
		fields = '__all__'