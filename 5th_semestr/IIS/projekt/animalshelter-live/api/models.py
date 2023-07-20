from django.db import models
from django.contrib.auth.models import AbstractUser
from django.utils import timezone
import datetime

class UserModel(AbstractUser):
	date_of_birth = models.DateField(null=True)
	photo = models.ImageField(null=True, upload_to='users')
	type = models.CharField(max_length=100, default='Registered')
	verified = models.BooleanField(default=False)
	
	class Meta:
		swappable = 'AUTH_USER_MODEL'

class AnimalModel(models.Model):
	name = models.CharField(max_length=100)
	breed = models.CharField(max_length=100)
	date_of_birth = models.DateField(null=True)
	photo = models.ImageField(null=True, upload_to='animals')
	description = models.TextField(null=True)
	found_info = models.TextField(null=True)
	sex = models.CharField(max_length=10, default="Unknown")

	def __str__(self):
		return self.name

class ScheduleModel(models.Model):
	date = models.DateField()
	s_from = models.TimeField(default='08:00')
	s_to = models.TimeField(default='14:00')

	animal = models.ForeignKey(AnimalModel, on_delete=models.CASCADE)

class ReservationModel(models.Model):
	r_from = models.TimeField(default='10:00')
	r_to = models.TimeField(default='12:00')
	borrowed = models.BooleanField(default=False)
	returned = models.BooleanField(default=False)
	confirmed = models.BooleanField(default=False)

	schedule = models.ForeignKey(ScheduleModel, on_delete=models.CASCADE)
	user = models.ForeignKey(UserModel, on_delete=models.CASCADE)

class RecordModel(models.Model):
	date = models.DateField()
	title = models.CharField(max_length=100)
	description = models.TextField(null=True)

	animal = models.ForeignKey(AnimalModel, on_delete=models.CASCADE)
	user = models.ForeignKey(UserModel, on_delete=models.SET_NULL, null=True)

	def __str__(self):
		return self.title

class RequestModel(models.Model):
	date = models.DateField()
	title = models.CharField(max_length=100)
	description = models.TextField(null=True)

	animal = models.ForeignKey(AnimalModel, on_delete=models.CASCADE)

	def __str__(self):
		return self.title

