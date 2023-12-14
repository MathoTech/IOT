<template>
  <div class="modal" v-if="showModal">
    <div class="modal-content">
      <b>Need Help?</b>
      <p>Is there any bug that you saw? Tell us!</p>
      <textarea
        placeholder="Content.."
        rows="4"
        cols="50"
        v-model="userInput"
      ></textarea>
      <div class="flex">
        <div class="close-button" @click="sendFeedback">
          <i class="material-icons">send</i>Send
        </div>
        <div class="close-button" @click="closeModal">
          <i class="material-icons">close</i>Close
        </div>
      </div>
      <p v-if="feedbackSent" class="feedback-confirmation">
        Feedback sent successfully!
      </p>
      <p v-if="feedbackSent == false" class="feedback-error">
        Feedback cannot be empty!
      </p>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      showModal: false,
      userInput: "",
      feedbackSent: null,
    };
  },
  methods: {
    openModal() {
      this.showModal = true;
    },
    closeModal() {
      this.showModal = false;
      this.$emit("modal-closed");
    },
    async sendFeedback() {
      if (this.userInput === "") {
        this.feedbackSent = false;
        return;
      }
    },
  },
};
</script>

<style scoped>
.feedback-error {
  padding-top: 16px;
  font-weight: bold;
  color: red;
}

.feedback-confirmation {
  padding-top: 16px;
  font-weight: bold;
}
.custom-input {
  width: 30%;
  background-color: transparent;
  color: white;
  border: 1px solid grey;
  border-radius: 5px;
  margin-bottom: 24px;
}

.custom-input:focus {
  border: 1px solid #eeeeee;
  outline: none;
}
.flex {
  display: flex;
  padding-top: 16px;
  justify-content: space-evenly;
}
i {
  padding-right: 8px;
  padding-bottom: 2px;
}
.close-button {
  background-color: #303030;
  padding: 8px 16px;
  border-radius: 5px;
  font-weight: bold;
  cursor: pointer;
  transition: 0.5s;
  color: #eeeeee;
}
.close-button:hover {
  background-color: #404040;
  transition: 0.5s;
}
textarea {
  background-color: #282828;
  color: #eeeeee;
  resize: none;
  border-radius: 5px;
}

textarea:focus {
  outline: none;
  border-color: #eeeeee;
}

b {
  color: #eeeeee;
  text-decoration: underline;
  font-size: 18px;
  padding-bottom: 8px;
}

p {
  color: #eeeeee;
}

.modal {
  animation: FadeIn 2s;
  position: fixed;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  background-color: #232323;
  padding: 20px;
  box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.5);
  border-radius: 5px;
  opacity: 0.9;
  max-width: 38%;
}

.modal-content {
  display: block;
  text-align: center;
}
</style>
