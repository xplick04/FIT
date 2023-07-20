from django.contrib import admin
from django.contrib.auth.admin import UserAdmin

from .models import (
	UserModel,
	AnimalModel,
	ScheduleModel,
	ReservationModel,
	RecordModel,
	RequestModel
)

admin.site.register(UserModel)
admin.site.register(AnimalModel)
admin.site.register(ScheduleModel)
admin.site.register(ReservationModel)
admin.site.register(RecordModel)
admin.site.register(RequestModel)
