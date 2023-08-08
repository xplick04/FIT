from django.urls import path

from .views import (
	registerUser,
	detailUser,
	listUser,
	deleteUser,
	verifyUser,
	typeUser,
	unverifiedUsers,
	uploadPhoto,
)

from .animal_views import (
	detailAnimal,
	createAnimal,
	listAnimal,
	deleteAnimal,
)

from .schedule_views import (
	createSchedule,
	listSchedule,
	deleteSchedule,
)

from .reservation_views import (
	createReservation,
	listReservation,
	deleteReservation,
	myWalks,
	updateReservation,
)

from .record_views import (
	createRecord,
	listRecord,
	deleteRecord,
)

from .request_views import (
	createRequest,
	listRequest,
	deleteRequest,
)

from rest_framework_simplejwt.views import (
    TokenRefreshView,
)

from .views import MyTokenObtainPairView

urlpatterns = [
	#jwt
	path('token/', MyTokenObtainPairView.as_view(), name='token_obtain_pair'),
    path('token/refresh/', TokenRefreshView.as_view(), name='token_refresh'),

	#user
	path('register/', registerUser),
	path('user-list/', listUser),
	path('user-unverified/', unverifiedUsers),

	path('user-detail/<str:pk>/', detailUser),
	path('user-delete/<str:pk>/', deleteUser),
	path('user-verify/<str:pk>/', verifyUser),
	path('user-type/<str:pk>/', typeUser),
	path('upload-photo/<str:pk>/', uploadPhoto),

	#animal
	path('animal-create/', createAnimal),
	path('animal-list/', listAnimal),

	path('animal-detail/<str:pk>/', detailAnimal),
	path('animal-delete/<str:pk>/', deleteAnimal),

	#schedule
	path('schedule-create/', createSchedule),
	path('schedule-list/<str:pk>/', listSchedule),

	#path('schedule-detail/<str:pk>/', detailSchedule),
	path('schedule-delete/<str:pk>/', deleteSchedule),

	#reservation
	path('reservation-create/', createReservation),
	path('mywalks/', myWalks),

	path('reservation-list/<str:pk>/', listReservation),
	path('reservation-delete/<str:pk>/', deleteReservation),
	path('reservation-update/<str:pk>/', updateReservation),

	#record
	path('record-create/', createRecord),
	path('record-list/<str:pk>/', listRecord),

	path('record-delete/<str:pk>/', deleteRecord),

	#request
	path('request-create/', createRequest),
	path('request-list/', listRequest),

	path('request-delete/<str:pk>/', deleteRequest),
]